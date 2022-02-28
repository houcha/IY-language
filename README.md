# Processor

ASM-like language compiler.

To build processor use `make build`. It generates necessary C++ code and executable files: `compile`, `run` and `disassembly`.
The `.asm` files (which are processor programs) located in `asm` directory. They can be executed in two actions:
1. `./compile asm/filename.asm`
2. `./run asm/filename.prc.o` Also additional flag `--make-listing` is available. It generates listing file: `asm/filename.lst`.

Processed programs (ones with `.prc.o` extension) can be disassembled with `./disassembly asm/filename.prc.o`. Disassembled file has `.dis.asm` extension.

