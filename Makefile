.PHONY: host riscv run clean sweep_host sweep_riscv generate_assembly vectorization_report 
# Default optimization level if not specified 
OPT_LEVEL ?= -O0 
# ────────────────────────────────────────────────────────────── 
#  Step 5: Vectorization parser — defined once, used by 
#          vectorization_report target via $VECTORIZATION_PARSER 
# ────────────────────────────────────────────────────────────── 
define VECTORIZATION_PARSER 
import sys, re, os 
from collections import defaultdict 
def parse_and_write(log_file, report_file, label): 
	if not os.path.exists(log_file): 
		print(f"SKIP: {log_file} not found (run the matching sweep target first).") 
		return 
	with open(log_file) as f: 
		lines = f.readlines() 
	levels  = ['O0', 'O1', 'O2', 'O3', 'Os', 'Ofast'] 
	current = None 
	buckets = {l: {'vectorized': [], 'missed': []} for l in levels} 
	for line in lines: 
		line = line.rstrip() 
		m = re.match(r'VECTORIZATION LOG: -(\w+)', line) 
		if m: 
			current = m.group(1) 
			continue 
		if current is None: 
			continue 
		if 'optimized: loop vectorized' in line: 
			buckets[current]['vectorized'].append(line) 
		elif 'src/' in line and 'missed:' in line: 
			buckets[current]['missed'].append(line) 
	out = open(report_file, 'w') 
	def w(s=''): 
		out.write(s + '\n') 
	w(f'=== PHASE 4 STEP 5: AUTO-VECTORIZATION ANALYSIS — {label} ===') 
	w(f'Source log  : {log_file}') 
	w(f'Report file : {report_file}') 
	w('Flags used  : -fopt-info-vec-all (applied during sweep)') 
	w() 
	w('System header misses are filtered out — only src/ failures shown.') 
	w('These are the loops in YOUR code that the compiler could not vectorize.') 
	w() 
	for level in levels: 
		vec = buckets[level]['vectorized'] 
		msd = buckets[level]['missed'] 
		w('=' * 60) 
		w(f'OPTIMIZATION LEVEL: -{level}') 
		w('=' * 60) 
		w() 
		w(f'[ VECTORIZED — {len(vec)} loop(s) successfully auto-vectorized ]') 
		if vec: 
			for line in vec: 
				w(f'  {line}') 
		else: 
			w('  (none at this level)') 
		w() 
		w(f'[ MISSED — {len(msd)} loop(s) in src/ failed to vectorize ]') 
		if msd: 
			by_file = defaultdict(list) 
			for line in msd: 
				fname = line.split(':')[0].strip() 
				by_file[fname].append(line) 
			for fname, flines in sorted(by_file.items()): 
				w(f'  {fname}:') 
				for fl in flines: 
					w(f'    {fl}') 
		else: 
			w('  (none — or vectorization not attempted at this level)') 
		w() 
	w() 
	w('=' * 60) 
	w('SUMMARY TABLE') 
	w('=' * 60) 
	w(f'  {"Level":<6} | {"Vectorized":>12} | {"Missed in src/":>16}') 
	w(f'  {"-"*6}-+-{"-"*12}-+-{"-"*16}') 
	for level in levels: 
		v = len(buckets[level]['vectorized']) 
		m = len(buckets[level]['missed']) 
		w(f'  -{level:<5} | {v:>12} | {m:>16}') 
	w() 
	w('WHAT THIS MEANS FOR YOUR REPORT:') 
	w('  - "control flow in loop"    -> boundary check prevents vectorization.') 
	w('    These are the Gaussian / Sobel inner loops.') 
	w('    Phase 6 RVV will handle these manually with intrinsics.') 
	w('  - "data dependency"         -> compiler cannot prove independent iterations.') 
	w('  - Loops that DO vectorize at -O3 show the compiler can help itself.') 
	w('  - Compare vectorized count at -O0 vs -O3 to show what -O3 unlocks.') 
	out.close() 
	print(f"Written: {report_file}") 
	with open(report_file) as f: 
		print(f.read()) 
parse_and_write('host_vectorization_log.txt',  'host_vectorization_report.txt',  'HOST (x86)') 
parse_and_write('riscv_vectorization_log.txt',  'riscv_vectorization_report.txt', 'RISC-V') 
endef 
export VECTORIZATION_PARSER 
# ────────────────────────────────────────────────────────────── 
#  Host Summary Table Parser Definition 
# ────────────────────────────────────────────────────────────── 
define HOST_SUMMARY_PARSER 
import json 
try: 
	state = json.load(open('host_sweep_state.json')) 
except Exception as e: 
	print(f"Could not read state: {e}"); exit(0) 
out = open('host_optimization_report.txt', 'a') 
def w(s=''): 
	out.write(s + '\n'); print(s) 
levels  = ['O0', 'O1', 'O2', 'O3', 'Os', 'Ofast'] 
present = [l for l in levels if l in state] 
if not present: 
	w('  (no data — all levels failed to compile)'); out.close(); exit(0) 
base_avg = state[present[0]]['avg_ms'] 
w(f'  {"Flag":<6} | {"text(bytes)":>11} | {"Total":>7} | {"Avg (5 runs)":>12} | {"Speedup vs O0":>13}') 
w(f'  {"-"*6}-+-{"-"*11}-+-{"-"*7}-+-{"-"*12}-+-{"-"*13}') 
best_ratio_level = None 
best_ratio = 0.0 
for level in levels: 
	if level not in state: 
		w(f'  -{level:<5} | {"FAILED":>11} | {"N/A":>7} | {"N/A":>12} | {"N/A":>13}') 
		continue 
	d = state[level] 
	speedup  = base_avg / d['avg_ms'] if d['avg_ms'] > 0 else 0 
	try: 
		size_kb = float(str(d['total_size']).replace('K','').replace('M','000')) 
	except: 
		size_kb = 1 
	ratio = speedup / size_kb if size_kb > 0 else 0 
	if ratio > best_ratio and level != 'O0': 
		best_ratio = ratio; best_ratio_level = level 
	marker = '  <- best perf/size' if level == best_ratio_level else '' 
	w(f'  -{level:<5} | {d["text_bytes"]:>11} | {d["total_size"]:>7} | {d["avg_ms"]:>10} ms | {speedup:>12.2f}x{marker}') 
w() 
if best_ratio_level: 
	w(f'  Best performance-per-size tradeoff: -{best_ratio_level} (Step 3 finding)') 
w(f'  Speedup formula: time_O0_avg / time_Ox_avg  |  base = {base_avg} ms') 
out.close() 
endef 
export HOST_SUMMARY_PARSER 
# ────────────────────────────────────────────────────────────── 
#  RISC-V Summary Table Parser Definition 
# ────────────────────────────────────────────────────────────── 
define RISCV_SUMMARY_PARSER 
import json, os 
try: 
	rv_state = json.load(open('riscv_sweep_state.json')) 
except Exception as e: 
	print(f"Could not read riscv state: {e}"); exit(0) 
try: 
	host_state = json.load(open('host_sweep_state.json')) 
	has_host = True 
except: 
	host_state = {}; has_host = False 
out = open('riscv_optimization_report.txt', 'a') 
def w(s=''): 
	out.write(s + '\n'); print(s) 
levels  = ['O0', 'O1', 'O2', 'O3', 'Os', 'Ofast'] 
present = [l for l in levels if l in rv_state] 
if not present: 
	w('  (no data — all levels failed to compile)'); out.close(); exit(0) 
base_avg = rv_state[present[0]]['avg_ms'] 
w(f'  {"Flag":<6} | {"Total":>7} | {"Avg (5 runs)":>12} | {"Speedup vs O0":>13}') 
w(f'  {"-"*6}-+-{"-"*7}-+-{"-"*12}-+-{"-"*13}') 
for level in levels: 
	if level not in rv_state: 
		w(f'  -{level:<5} | {"N/A":>7} | {"N/A":>12} | {"N/A":>13}') 
		continue 
	d = rv_state[level] 
	speedup = base_avg / d['avg_ms'] if d['avg_ms'] > 0 else 0 
	w(f'  -{level:<5} | {d["total_size"]:>7} | {d["avg_ms"]:>10} ms | {speedup:>12.2f}x') 
w() 
w(f'  Speedup formula: time_O0_avg / time_Ox_avg  |  base = {base_avg} ms') 
if has_host: 
	w() 
	w('=' * 60) 
	w('HOST vs RISC-V COMPARISON (Step 7)') 
	w('=' * 60) 
	w(f'  {"Flag":<6} | {"Host avg":>10} | {"RISC-V avg":>12} | {"RISC-V overhead":>16}') 
	w(f'  {"-"*6}-+-{"-"*10}-+-{"-"*12}-+-{"-"*16}') 
	for level in levels: 
		if level in host_state and level in rv_state: 
			h = host_state[level]['avg_ms'] 
			r = rv_state[level]['avg_ms'] 
			overhead = r / h if h > 0 else 0 
			w(f'  -{level:<5} | {h:>8} ms | {r:>10} ms | {overhead:>14.1f}x slower') 
	w() 
	w('  NOTE: RISC-V runs slower than host because QEMU translates every') 
	w('  RISC-V instruction to x86 at runtime. This is expected. The') 
	w('  relative speedup pattern across -O levels should be similar on both.') 
else: 
	w() 
	w('  Host vs RISC-V comparison: run make sweep_host first to enable this.') 
out.close() 
endef 
export RISCV_SUMMARY_PARSER 
# ────────────────────────────────────────────────────────────── 
#  Single-level builds 
# ────────────────────────────────────────────────────────────── 
host: 
	@OPT_LEVEL=$(OPT_LEVEL) ./scripts/build_host.sh 
riscv: 
	@OPT_LEVEL=$(OPT_LEVEL) ./scripts/build_riscv.sh 
run: 
	./scripts/run_qemu.sh 
run_host: 
	build-host/rv_project_host
	./build-host/rv_project_host
clean: 
	rm -rf build-host build-riscv \
		host_optimization_report.txt riscv_optimization_report.txt \
		host_vectorization_log.txt riscv_vectorization_log.txt \
		host_vectorization_report.txt riscv_vectorization_report.txt \
		host_sweep_state.json riscv_sweep_state.json 

# ======================================================================
define PADDING_SCRIPT
import re, os
from collections import defaultdict

def parse_vec_log(filename):
    if not os.path.exists(filename):
        return [], []
    vectorized, missed = [], []
    with open(filename) as f:
        for line in f:
            line = line.rstrip()
            if 'optimized: loop vectorized' in line:
                vectorized.append(line)
            elif 'src/' in line and 'missed:' in line:
                # EXCLUDE structural noise lines from the total count
                if 'epilogue' in line or 'versioning' in line or 'alignment' in line:
                    continue
                missed.append(line)
    return vectorized, missed

before_vec, before_miss = parse_vec_log('padding_before_vec.txt')
after_vec,  after_miss  = parse_vec_log('padding_after_vec.txt')

newly_vectorized = [l for l in after_vec  if l not in before_vec]
still_missed     = [l for l in after_miss if l not in before_miss]
fixed_loops      = [l for l in before_miss if l not in after_miss]

out = open('padding_comparison.txt', 'w')
def w(s=''):
    out.write(s + '\n'); print(s)

w('=== PHASE 4: HOST PADDING EXPERIMENT — COMPARISON REPORT ===')
w('Technique: pre-pad image with zeros before convolution')
w('Flag:      -DUSE_PADDED_CONV')
w('File:      include/filters/convolution.hpp')
w()
w('=' * 60)
w('SUMMARY')
w('=' * 60)
w(f'  {"":30} {"BEFORE":>8}  {"AFTER":>8}  {"CHANGE":>8}')
w(f'  {"-"*30}-+-{"-"*8}-+-{"-"*8}-+-{"-"*8}')
w(f'  {"Loops vectorized (src/)":30} {len(before_vec):>8}  {len(after_vec):>8}  {len(after_vec)-len(before_vec):>+8}')
w(f'  {"Loops missed (src/)":30} {len(before_miss):>8}  {len(after_miss):>8}  {len(after_miss)-len(before_miss):>+8}')
w()
w('=' * 60)
w(f'LOOPS FIXED BY PADDING ({len(fixed_loops)} loop(s) no longer missed)')
w('=' * 60)
if fixed_loops:
    by_file = defaultdict(list)
    for line in fixed_loops:
        fname = line.split(':')[0].strip()
        by_file[fname].append(line)
    for fname, lines in sorted(by_file.items()):
        w(f'  {fname}:')
        for l in lines:
            w(f'    {l}')
else:
    w('  (none)')
w()
w('=' * 60)
w(f'NEWLY VECTORIZED LOOPS ({len(newly_vectorized)} loop(s) now auto-vectorized)')
w('=' * 60)
if newly_vectorized:
    for l in newly_vectorized:
        w(f'  {l}')
else:
    w('  (none at this level)')

out.close()
print()
print("Written: padding_comparison.txt")
endef
export PADDING_SCRIPT
# =========================================================================
define PADDING_SCRIPT_RISCV
import re, os
from collections import defaultdict

def parse_vec_log(filename):
    if not os.path.exists(filename):
        return [], []
    vectorized, missed = [], []
    with open(filename) as f:
        for line in f:
            line = line.rstrip()
            if 'optimized: loop vectorized' in line:
                vectorized.append(line)
            elif 'src/' in line and 'missed:' in line:
                # EXCLUDE structural noise lines from the total count
                if 'epilogue' in line or 'versioning' in line or 'alignment' in line:
                    continue
                missed.append(line)
    return vectorized, missed

before_vec, before_miss = parse_vec_log('padding_before_vec_riscv.txt')
after_vec,  after_miss  = parse_vec_log('padding_after_vec_riscv.txt')

newly_vectorized = [l for l in after_vec  if l not in before_vec]
still_missed     = [l for l in after_miss if l not in before_miss]
fixed_loops      = [l for l in before_miss if l not in after_miss]

out = open('padding_comparison_riscv.txt', 'w')
def w(s=''):
    out.write(s + '\n'); print(s)

w('=== PHASE 4: RISC-V PADDING EXPERIMENT — COMPARISON REPORT ===')
w('Technique: pre-pad image with zeros before convolution (RISC-V)')
w('Flag:      -DUSE_PADDED_CONV')
w('File:      include/filters/convolution.hpp')
w()
w('=' * 60)
w('SUMMARY (RISC-V)')
w('=' * 60)
w(f'  {"":30} {"BEFORE":>8}  {"AFTER":>8}  {"CHANGE":>8}')
w(f'  {"-"*30}-+-{"-"*8}-+-{"-"*8}-+-{"-"*8}')
w(f'  {"Loops vectorized (src/)":30} {len(before_vec):>8}  {len(after_vec):>8}  {len(after_vec)-len(before_vec):>+8}')
w(f'  {"Loops missed (src/)":30} {len(before_miss):>8}  {len(after_miss):>8}  {len(after_miss)-len(before_miss):>+8}')
w()
w('=' * 60)
w(f'LOOPS FIXED BY PADDING ({len(fixed_loops)} loop(s) no longer missed)')
w('=' * 60)
if fixed_loops:
    by_file = defaultdict(list)
    for line in fixed_loops:
        fname = line.split(':')[0].strip()
        by_file[fname].append(line)
    for fname, lines in sorted(by_file.items()):
        w(f'  {fname}:')
        for l in lines:
            w(f'    {l}')
else:
    w('  (none)')
w()
w('=' * 60)
w(f'NEWLY VECTORIZED LOOPS ({len(newly_vectorized)} loop(s) now auto-vectorized)')
w('=' * 60)
if newly_vectorized:
    for l in newly_vectorized:
        w(f'  {l}')
else:
    w('  (none at this level)')

out.close()
print()
print("Written: padding_comparison_riscv.txt")
endef
export PADDING_SCRIPT_RISCV
# ────────────────────────────────────────────────────────────── 
#  Phase 4 – Steps 2, 3, 5, 6: Host sweep 
# ────────────────────────────────────────────────────────────── 
sweep_host: 
	@echo "{}" > host_sweep_state.json 
	@echo ""   > host_vectorization_log.txt 
	@echo "=== PHASE 4: HOST OPTIMIZATION SWEEP ===" > host_optimization_report.txt 
	@echo "Started: $$(date)"                  >> host_optimization_report.txt 
	@echo "Binary:  build-host/rv_project_host"        >> host_optimization_report.txt 
	@echo "Runs per level: 5"                 >> host_optimization_report.txt 
	@echo ""                                 >> host_optimization_report.txt 
	@for opt in O0 O1 O2 O3 Os Ofast; do \
		echo ""; \
		echo ">>> Building -$$opt ..."; \
		echo "==================================================" >> host_optimization_report.txt; \
		echo "OPTIMIZATION LEVEL: -$$opt"                 >> host_optimization_report.txt; \
		echo "==================================================" >> host_optimization_report.txt; \
		echo ""                                       >> host_vectorization_log.txt; \
		echo "==================================================" >> host_vectorization_log.txt; \
		echo "VECTORIZATION LOG: -$$opt"                 >> host_vectorization_log.txt; \
		echo "==================================================" >> host_vectorization_log.txt; \
		if [ -d build-host ]; then find build-host -mindepth 1 ! -name 'rv_project_host_*' -delete; fi; \
		if OPT_LEVEL="-$$opt -fopt-info-vec-all" ./scripts/build_host.sh >> host_vectorization_log.txt 2>&1; then \
			echo "Compilation: SUCCESS"              >> host_optimization_report.txt; \
			echo "    Compilation: SUCCESS"; \
		else \
			echo "Compilation: FAILED - skipping level"  >> host_optimization_report.txt; \
			echo "    Compilation: FAILED - skipping"; \
			echo "" >> host_optimization_report.txt; \
			continue; \
		fi; \
		cp build-host/rv_project_host build-host/rv_project_host_$$opt; \
		echo ""                                       >> host_optimization_report.txt; \
		echo "[ Binary Size - Section Breakdown (text/data/bss) ]" >> host_optimization_report.txt; \
		size build-host/rv_project_host                >> host_optimization_report.txt 2>&1; \
		echo ""                                       >> host_optimization_report.txt; \
		echo "[ Binary Size - Total File ]"             >> host_optimization_report.txt; \
		ls -lh build-host/rv_project_host \
			| awk '{print "  Size: " $$5 "  (" $$9 ")"}' \
			| sed 's/\x1b\[0-9;]*m//g' >> host_optimization_report.txt; \
		echo ""                                       >> host_optimization_report.txt; \
		echo "[ Execution Time (internal measurement) ]" >> host_optimization_report.txt; \
        val=$$(./build-host/rv_project_host | grep "MEASURED_TIME_MS" | cut -d' ' -f2); \
        echo "  Execution time: $$val ms" >> host_optimization_report.txt; \
        echo "      Execution time: $$val ms"; \
		echo ""                                       >> host_optimization_report.txt; \
		echo "[ Wall-Clock Timing (5 runs: min / avg / max) ]" >> host_optimization_report.txt; \
		echo "  Five runs for stable statistics — used for speedup comparison." >> host_optimization_report.txt; \
		echo "  (python3 time.perf_counter, ms resolution)" >> host_optimization_report.txt; \
		echo "    Timing 5 runs ..."; \
		python3 -c "import subprocess, json, re; \
ts = []; \
[ts.append(float(re.search(r'MEASURED_TIME_MS:\s*([0-9.]+)', subprocess.run(['./build-host/rv_project_host'], capture_output=True, text=True).stdout).group(1))) for _ in range(5)]; \
[print(f'      Run {i+1}: {t:.3f} ms') or open('host_optimization_report.txt','a').write(f'  Run {i+1}: {t:.3f} ms\n') for i,t in enumerate(ts)]; \
avg = round(sum(ts)/len(ts),3); mn = min(ts); mx = max(ts); \
open('host_optimization_report.txt','a').write(f'\n  Min: {mn:.3f} ms  |  Avg: {avg:.3f} ms  |  Max: {mx:.3f} ms\n'); \
state = json.load(open('host_sweep_state.json')); \
lines = open('host_optimization_report.txt').readlines(); \
text_bytes = next((int(l.split()[0]) for l in reversed(lines) if l.strip() and l.strip().split() and l.strip().split()[0].isdigit() and 'text' not in l and 'Run' not in l and 'ms' not in l), 0); \
total_size = next((l.strip().split()[1] for l in reversed(lines) if 'Size:' in l), 'N/A'); \
state['$$opt'] = {'avg_ms': avg, 'min_ms': mn, 'max_ms': mx, 'text_bytes': text_bytes, 'total_size': total_size}; \
json.dump(state, open('host_sweep_state.json','w'))"; \
		echo ""                                       >> host_optimization_report.txt; \
		echo "  NOTE: QEMU is not cycle-accurate. Absolute values are not"   >> host_optimization_report.txt; \
		echo "  meaningful; only relative comparisons between levels are."   >> host_optimization_report.txt; \
		echo "" >> host_optimization_report.txt; \
	done 
	@echo ""                                 >> host_optimization_report.txt 
	@echo "=== AUTO-FILLED SUMMARY TABLE ==="           >> host_optimization_report.txt 
	@echo "Generated automatically from sweep data."   >> host_optimization_report.txt 
	@echo ""                                 >> host_optimization_report.txt 
	@python3 -c "import os; exec(os.environ['HOST_SUMMARY_PARSER'])" 
	@echo "" 
	@echo "====================================================================" 
	@echo "Host sweep done." 
	@echo "Output files:" 
	@echo "  host_optimization_report.txt   <- sizes + timing + auto summary" 
	@echo "  host_vectorization_log.txt     <- raw -fopt-info-vec-all output" 
	@echo "  host_sweep_state.json          <- internal state (safe to ignore)" 
	@echo "Saved binaries: build-host/rv_project_host_{O0,O1,O2,O3,Os,Ofast}" 
	@echo "Next: make vectorization_report   then   make generate_assembly" 
	@echo "====================================================================" 
# ────────────────────────────────────────────────────────────── 
#  Phase 4 – Step 5: Parse vectorization logs → clean reports 
# ────────────────────────────────────────────────────────────── 
vectorization_report: 
	@echo ">>> Parsing vectorization logs ..." 
	@python3 -c "import os; exec(os.environ['VECTORIZATION_PARSER'])" 
# ────────────────────────────────────────────────────────────── 
#  Phase 4 – Step 7: RISC-V / QEMU sweep 
# ────────────────────────────────────────────────────────────── 
sweep_riscv: 
	@echo "{}" > riscv_sweep_state.json 
	@echo ""   > riscv_vectorization_log.txt 
	@echo "=== PHASE 4: RISC-V / QEMU OPTIMIZATION SWEEP ===" > riscv_optimization_report.txt 
	@echo "Started: $$(date)"                             >> riscv_optimization_report.txt 
	@echo "Binary:   build-riscv/rv_project_riscv"             >> riscv_optimization_report.txt 
	@echo "QEMU:     rv64,v=true,vlen=128"                 >> riscv_optimization_report.txt 
	@echo "Runs per level: 5"                             >> riscv_optimization_report.txt 
	@echo ""                                             >> riscv_optimization_report.txt 
	@for opt in O0 O1 O2 O3 Os Ofast; do \
		echo ""; \
		echo ">>> Cross-compiling -$$opt for RISC-V ..."; \
		echo "==================================================" >> riscv_optimization_report.txt; \
		echo "OPTIMIZATION LEVEL: -$$opt"                   >> riscv_optimization_report.txt; \
		echo "==================================================" >> riscv_optimization_report.txt; \
		echo ""                                             >> riscv_vectorization_log.txt; \
		echo "==================================================" >> riscv_vectorization_log.txt; \
		echo "VECTORIZATION LOG: -$$opt"                    >> riscv_vectorization_log.txt; \
		echo "==================================================" >> riscv_vectorization_log.txt; \
		if [ -d build-riscv ]; then find build-riscv -mindepth 1 ! -name 'rv_project_riscv_*' -delete; fi; \
		if OPT_LEVEL="-$$opt -fopt-info-vec-all" ./scripts/build_riscv.sh >> riscv_vectorization_log.txt 2>&1; then \
			echo "Compilation: SUCCESS"                     >> riscv_optimization_report.txt; \
			echo "    Compilation: SUCCESS"; \
		else \
			echo "Compilation: FAILED - skipping level"     >> riscv_optimization_report.txt; \
			echo "    Compilation: FAILED - skipping"; \
			echo "" >> riscv_optimization_report.txt; \
			continue; \
		fi; \
		cp build-riscv/rv_project_riscv build-riscv/rv_project_riscv_$$opt; \
		echo ""                                             >> riscv_optimization_report.txt; \
		echo "[ Binary Size - Section Breakdown ]"           >> riscv_optimization_report.txt; \
		size build-riscv/rv_project_riscv                   >> riscv_optimization_report.txt 2>&1; \
		echo ""                                             >> riscv_optimization_report.txt; \
		echo "[ Binary Size - Total File ]"                 >> riscv_optimization_report.txt; \
		ls -lh build-riscv/rv_project_riscv \
			| awk '{print "  Size: " $$5 "  (" $$9 ")"}' \
			| sed 's/\x1b\[0-9;]*m//g' >> riscv_optimization_report.txt; \
		echo ""                                             >> riscv_optimization_report.txt; \
		echo "[ Step 2 - Execution Time (single run on QEMU) ]" >> riscv_optimization_report.txt; \
		echo "  One wall-clock measurement per level — used for the Step 2 table." >> riscv_optimization_report.txt; \
		python3 -c "\
import subprocess, re; \
out = subprocess.run(['qemu-riscv64', '-cpu', 'rv64,v=true,vlen=128', './build-riscv/rv_project_riscv'], capture_output=True, text=True).stdout; \
dt = float(re.search(r'MEASURED_TIME_MS:\s*([0-9.]+)', out).group(1)); \
open('riscv_optimization_report.txt', 'a').write(f'  Execution time: {dt:.3f} ms\n'); \
print(f'      Execution time: {dt:.3f} ms')"; \
		echo ""                                             >> riscv_optimization_report.txt; \
		echo "[ Step 6 - Wall-Clock Timing on QEMU (5 runs: min / avg / max) ]" >> riscv_optimization_report.txt; \
		echo "  (qemu-riscv64 -cpu rv64,v=true,vlen=128)" >> riscv_optimization_report.txt; \
		echo "    Timing 5 runs on QEMU ..."; \
		python3 -c "\
import subprocess, json, re; \
ts = []; \
[ts.append(float(re.search(r'MEASURED_TIME_MS:\s*([0-9.]+)', subprocess.run(['qemu-riscv64', '-cpu', 'rv64,v=true,vlen=128', './build-riscv/rv_project_riscv'], capture_output=True, text=True).stdout).group(1))) for _ in range(5)]; \
[print(f'      Run {i+1}: {t:.3f} ms') or open('riscv_optimization_report.txt','a').write(f'  Run {i+1}: {t:.3f} ms\n') for i,t in enumerate(ts)]; \
avg = round(sum(ts)/5, 3); mn = min(ts); mx = max(ts); \
open('riscv_optimization_report.txt','a').write(f'\n  Min: {mn:.3f} ms  |  Avg: {avg:.3f} ms  |  Max: {mx:.3f} ms\n'); \
state = json.load(open('riscv_sweep_state.json')); \
lines = open('riscv_optimization_report.txt').readlines(); \
total_size = next((l.strip().split()[1] for l in reversed(lines) if 'Size:' in l), 'N/A'); \
state['$$opt'] = {'avg_ms': avg, 'min_ms': mn, 'max_ms': mx, 'total_size': total_size}; \
json.dump(state, open('riscv_sweep_state.json','w'))"; \
		echo ""                                             >> riscv_optimization_report.txt; \
		echo "  NOTE: QEMU is not cycle-accurate. Only relative comparisons" >> riscv_optimization_report.txt; \
		echo "  between -O levels are valid (instruction count changes)."    >> riscv_optimization_report.txt; \
		echo "" >> riscv_optimization_report.txt; \
	done 
	@echo ""                                               >> riscv_optimization_report.txt 
	@echo "=== AUTO-FILLED SUMMARY TABLE ==="              >> riscv_optimization_report.txt 
	@echo "Generated automatically from sweep data."       >> riscv_optimization_report.txt 
	@echo ""                                               >> riscv_optimization_report.txt 
	@python3 -c "import os; exec(os.environ['RISCV_SUMMARY_PARSER'])" 
	@echo "" 
	@echo "====================================================================" 
	@echo "RISC-V sweep done." 
	@echo "Output files:" 
	@echo "  riscv_optimization_report.txt  <- sizes + timing + auto summary" 
	@echo "  riscv_vectorization_log.txt    <- raw -fopt-info-vec-all output" 
	@echo "  riscv_sweep_state.json         <- internal state (safe to ignore)" 
	@echo "Saved binaries: build-riscv/rv_project_riscv_{O0,O1,O2,O3,Os,Ofast}" 
	@echo "Next: make vectorization_report   then   make generate_assembly" 
	@echo "===================================================================="
# ────────────────────────────────────────────────────────────── 
#  Phase 4 – Step 4: Assembly extraction for all 4 levels 
# ────────────────────────────────────────────────────────────── 
generate_assembly:
	@echo ">>> Extracting Host Assembly for all optimization levels (-O0 through -O3)..."
	@for opt in O0 O1 O2 O3 Os Ofast; do \
		if [ -f build-host/rv_project_host_$$opt ]; then \
			objdump -d build-host/rv_project_host_$$opt > build-host/host_assembly_$$opt.s 2>/dev/null; \
			echo "  [Host] Generated build-host/host_assembly_$$opt.s"; \
		else \
			echo "  WARNING: build-host/rv_project_host_$$opt missing — run make sweep_host first"; \
		fi; \
	done
	@echo ">>> Extracting RISC-V Assembly for all optimization levels (-O0 through -O3)..."
	@for opt in O0 O1 O2 O3 Os Ofast; do \
		if [ -f build-riscv/rv_project_riscv_$$opt ]; then \
			riscv64-unknown-elf-objdump -d build-riscv/rv_project_riscv_$$opt > build-riscv/riscv_assembly_$$opt.s 2>/dev/null \
			|| riscv64-linux-gnu-objdump -d build-riscv/rv_project_riscv_$$opt > build-riscv/riscv_assembly_$$opt.s 2>/dev/null \
			|| objdump -d build-riscv/rv_project_riscv_$$opt > build-riscv/riscv_assembly_$$opt.s 2>/dev/null; \
			echo "  [RISC-V] Generated build-riscv/riscv_assembly_$$opt.s"; \
		else \
			echo "  WARNING: build-riscv/rv_project_riscv_$$opt missing — run make sweep_riscv first"; \
		fi; \
	done
	@echo "" >> riscv_optimization_report.txt
	@echo "=== STEP 4: AUTOMATED VSET INSTRUCTION COUNT ===" >> riscv_optimization_report.txt
	@for opt in O0 O1 O2 O3 Os Ofast; do \
		if [ -f build-riscv/riscv_assembly_$$opt.s ]; then \
			COUNT=$$(grep -c 'vset' build-riscv/riscv_assembly_$$opt.s); \
			echo "  -$$opt Vector Instructions (vset count): $$COUNT" >> riscv_optimization_report.txt; \
			echo "  -$$opt Vector Instructions (vset count): $$COUNT"; \
		fi; \
	done

# ──────────────────────────────────────────────────────────────
#  Phase 4 – Deeper Idea: Padding Experiment
#
#  Demonstrates that code structure (not just the algorithm) affects
#  auto-vectorizability. Builds the same -O3 binary twice:
#    BEFORE: standard convolve() with boundary check  (no -DUSE_PADDED_CONV)
#    AFTER:  convolve_padded() via macro redirect      (-DUSE_PADDED_CONV)
#
#  The only source change is in convolution.hpp — a #define redirect
#  activated by -DUSE_PADDED_CONV. gaussian.cpp is untouched.
#
#  Writes:
#    padding_before_vec.txt   — raw -fopt-info-vec-all log without padding
#    padding_after_vec.txt    — raw -fopt-info-vec-all log with padding
#    padding_comparison.txt   — clean diff showing what changed
#
#  Run AFTER make sweep_host (needs build-host/ directory to exist).
# ──────────────────────────────────────────────────────────────
padding_experiment:
	@echo ">>> Phase 4 Deeper Idea: Padding Experiment"
	@echo ">>> Step 1/3: Building WITHOUT padding (-O3, boundary check) ..."
	@OPT_LEVEL="-O3 -fopt-info-vec-all" ./scripts/build_host.sh > padding_before_vec.txt 2>&1 \
		&& echo "    Build WITHOUT padding: SUCCESS" \
		|| echo "    Build WITHOUT padding: FAILED"
	@echo ">>> Step 2/3: Building WITH padding (-O3 -DUSE_PADDED_CONV) ..."
	@OPT_LEVEL="-O3 -fopt-info-vec-all -DUSE_PADDED_CONV" ./scripts/build_host.sh > padding_after_vec.txt 2>&1 \
		&& echo "    Build WITH padding: SUCCESS" \
		|| echo "    Build WITH padding: FAILED"
	@echo ">>> Step 3/3: Generating comparison report ..."
	@python3 -c "$$PADDING_SCRIPT"
	@echo ""
	@echo "===================================================================="
	@echo "Padding experiment done."
	@echo "  padding_before_vec.txt   <- raw vec log (boundary check)"
	@echo "  padding_after_vec.txt    <- raw vec log (padded)"
	@echo "  padding_comparison.txt   <- clean diff for your report"
	@echo "===================================================================="

padding_experiment_riscv:
	@echo ">>> Phase 4 Deeper Idea: RISC-V Padding Experiment"
	@echo ">>> Step 1/3: Building RISC-V WITHOUT padding (-O3, boundary check) ..."
	@OPT_LEVEL="-O3 -fopt-info-vec-all" ./scripts/build_riscv.sh > padding_before_vec_riscv.txt 2>&1 \
		&& echo "    RISC-V Build WITHOUT padding: SUCCESS" \
		|| echo "    RISC-V Build WITHOUT padding: FAILED"
	@echo ">>> Step 2/3: Building RISC-V WITH padding (-O3 -DUSE_PADDED_CONV) ..."
	@OPT_LEVEL="-O3 -fopt-info-vec-all -DUSE_PADDED_CONV" ./scripts/build_riscv.sh > padding_after_vec_riscv.txt 2>&1 \
		&& echo "    RISC-V Build WITH padding: SUCCESS" \
		|| echo "    RISC-V Build WITH padding: FAILED"
	@echo ">>> Step 3/3: Generating RISC-V comparison report ..."
	@python3 -c "$$PADDING_SCRIPT_RISCV"
	@echo ""
	@echo "===================================================================="
	@echo "RISC-V Padding experiment done."
	@echo "  padding_before_vec_riscv.txt <- raw vec log (boundary check)"
	@echo "  padding_after_vec_riscv.txt  <- raw vec log (padded)"
	@echo "  padding_comparison_riscv.txt <- clean diff for your report"
	@echo "===================================================================="