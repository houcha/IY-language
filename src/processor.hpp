#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include <cstdio>                   // printf
#include <cmath>                    // sqrt
#include <limits>                   // infinity
#include "text.hpp"                 // GetFileSize
#include "assembly_reader.hpp"      // GetLexeme
#include "command.gen.hpp"          // Command
#include "disassembler.hpp"         // UnpackArg for listing
#include "args.hpp"


void Run(FILE* assembled, FILE* listing);


// Heap is much greater than stack.
template <int FullMemorySize = 100000000, int StackSize = 1000000,
          typename StackValueType = uint64_t>
class Processor {

  private:

/*
          Memory structure:

    [data, text, __ram__, __stack]

    pc ---^
    stack_frame --------^       ^------ sp
*/

    /// Full allocated memory.
    char* memory;
    /// Ptr to data section begin.
    char* data;
    /// Ptr to text section begin.
    char* text;
    /// Ptr to ram begin.
    char* ram;
    /// Ptr to stack-ram border (from ram size).
    char* stack_frame;

    /// Stack pointer.
    char* sp;
    /// Program counter.
    char* pc;

    /// 12 64-bit registers. 13th is for special use.
    int64_t r[13];
    /// 8 64-bit floating point registers.
    double xmm[8];

    /// Flags.
    bool ZF;
    bool SF;
    bool CF;
    bool OF;

  public:

    Processor();
    ~Processor();

    void LoadProgram(FILE* assembled);
    void Execute(FILE* listing = nullptr);

  private:

    #include "processor_commands.inl"
    #include "processor_execute_command.gen.inl"

    void PrintArgValue(FILE* listing, uint64_t arg, int arg_type);
    void PrintToListing(FILE* listing, Command& command,
                        uint64_t left_arg, uint64_t right_arg, char* pc);
};

#include "processor.inl"


#endif // PROCESSOR_HPP

