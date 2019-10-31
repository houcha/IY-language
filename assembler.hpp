#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include <iostream> // std::cerr
#include <cstdio>   // FILE
#include <vector>
#include <string>
#include <unordered_map>
#include "text/text.hpp"
#include "command.hpp"
//#include "hash_table.hpp"


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

    void PackCommand(Command& command);

    template <typename String>
    void PackArg(const String& arg, int type);

    template <typename String>
    int GetArgType(const String& arg);
};


void Disassembly(FILE* src, FILE* dest);


class AssemblyReader : public Text {

  private:

    size_t current_pos_;

  public:

    AssemblyReader() : Text(),
                       current_pos_(0) {}

    /// Return next lexeme in raw.
    char&  GetLexeme()      { return buffer_[current_pos_++]; }
    size_t GetCurrentPos()  { return current_pos_; }
    /// Return true if code read is finished.
    bool   IsFinish()       { return current_pos_ == buffer_.size(); }
    /// Move carriage to pos.
    void   GoTo(size_t pos) { current_pos_ = pos; }
};


#endif // ASSEMBLER_HPP

