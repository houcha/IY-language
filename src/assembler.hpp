#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include <iostream> // std::cerr
#include <cstdio>   // FILE
#include <cstring>  // memmove
#include <vector>
#include <string>
#include <unordered_map>
#include "text.hpp"
#include "command.gen.hpp"
#include "args.hpp"


void Compile(FILE* src, FILE* dest);


class Assembly {

  public:

    typedef char value_type;

  private:

    size_t current_pos_;
    std::vector<value_type> data_;
    /// Need while assembly.
    std::unordered_map<std::string_view, size_t> labels_table_;

  public:

    Assembly() : current_pos_(0),
                 data_(),
                 labels_table_() {}

    /// Construct data on src file content.
    void Parse(FILE* src);
    /// Writes data to file.
    void Write(FILE* dest);

  private:
    /// Fill labels hash table need for Assembly.
    void PreAssembly(const Text& text);

    template <typename Line>
    void ParseLine(const Line& line);

    template <typename T>
    void Pack(T&& object);

    template <typename String>
    void PackArg(const String& arg, int type);
};


#endif // ASSEMBLER_HPP

