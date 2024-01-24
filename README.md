# Pico-RISC Assembler

## Description
This project is an assembler designed for the Pico-RISC ISA, created by [sderrien](https://github.com/sderrien/).

## Compilation
To compile the assembler, follow these steps:

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

## Usage
To use the assembler, execute the following command:

```bash
./build/bin/asm [input file] [output file]
```
Replace [input file] with the path to the source code file you want to assemble and [output file] with the desired name for the resulting binary file.
