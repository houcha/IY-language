#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include "stack/stack.hpp"
#include "assembler.hpp"


void Run(FILE* assembled);

// Let heap be 100 greater than stack.
template <int FullMemorySize = 100000000, int StackSize = 1000000>
class Processor {

  private:

    /// Full allocated memory.
    void* memory_;
    /// Stack.
    StaticStack<int, StackSize> stack_;

    /// 16 of 32-bit registers.
    int32_t r[16];
    bool ZF;
    bool SF;
    bool CF;
    bool OF;

  public:

    Processor() : memory_(malloc(FullMemorySize * sizeof(char))),
                  stack_() {}

    ~Processor() { free(memory_); }

    int ExecuteCommand(const Command& command,
                       int8_t left_arg, int8_t right_arg,
                       const AssemblyReader& reader);
};


#endif // PROCESSOR_HPP

