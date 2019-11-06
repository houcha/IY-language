#ifndef DISASSEMBLER_HPP
#define DISASSEMBLER_HPP

#include <iostream> // cerr
#include <string>
#include <cstdio>
#include "text.hpp"
#include "assembly_reader.hpp"
#include "args.hpp"
#include "command.gen.hpp"


std::string UnpackArg(uint64_t arg, int arg_type);

void Disassembly(FILE* src, FILE* dest);


#endif // DISASSEMBLER_HPP

