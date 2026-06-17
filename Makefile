.PHONY: host riscv run clean

host:
	./scripts/build_host.sh

riscv:
	./scripts/build_riscv.sh

run:
	./scripts/run_qemu.sh

clean:
	rm -rf build-host build-riscv
