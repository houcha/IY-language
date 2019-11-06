# Processor.

To build processor use `build` target of `Makefile`. It generates necessary C++ code and executable files: `compile`, `run` and `disassembly`.
The `.asm` files (which are processor programs) located in `asm` directory. They can be executed in two actions:
1. `./compile asm/filename.asm`
2. `./run asm/filename.prc.o`

Processed programs (one with `.prc.o` extension) can be disassembled with `./disassembly asm/filename.prc.o`. Disassembled file has `.dis.asm` extension.

