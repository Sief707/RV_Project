.PHONY: host riscv test run clean

host:
	./scripts/build_host.sh

riscv:
	./scripts/build_riscv.sh

test:
	./scripts/build_tests.sh
	./scripts/run_tests.sh

run:
	./scripts/run_qemu.sh

clean:
	rm -rf build-host build-riscv
