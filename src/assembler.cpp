#include "assembler.hpp"


void Compile(FILE* src, FILE* dest) {
  Assembly assembly;

  assembly.Parse(src);
  assembly.Write(dest);
}

template <typename String>
bool IsLabel(const String& word) {
  return word.back() == ':';
}

void Assembly::PreAssembly(const Text& text) {
  current_pos_ = 0;
  for (auto& line : text.Lines()) {
    // If word is label, it is first word, so check line[0] only.
    // If word ends in ':', it is label.
    if (IsLabel(line[0])) {
      // ':' is not a part of label name, so remove it.
      std::string_view label_name(line[0].data(), line[0].size() - 1);
      labels_table_[label_name] = current_pos_;
    }
    current_pos_ += sizeof(Command) + (line.size() - 1) * sizeof(Arg);
  }
}

void Assembly::Parse(FILE* src) {
  Text src_text;
  src_text.Load(src);
  src_text.SplitToLines();

  PreAssembly(src_text);
  // current_pos_ is correct size from PreAssembly.
  data_.resize(current_pos_, 0);

  current_pos_ = 0;
  for (auto& line : src_text.Lines()) {
    ParseLine(line);
  }
}

void Assembly::Write(FILE* dest) {
  fwrite(data_.data(), sizeof(value_type), data_.size(), dest);
}

template <typename Line>
void Assembly::ParseLine(const Line& line) {
  // Skip labels.
  if (IsLabel(line[0])) { current_pos_ += sizeof(Command); return; }
  // Create command.
  Command command(line);
  // Push it to data.
  Pack(command);

  if (command.HasLeftArg()) {
    PackArg(line[1], command.left_arg_type);
  }
  if (command.HasRightArg()) {
    PackArg(line[2], command.right_arg_type);
  }
}

template <typename T>
void Assembly::Pack(T&& object) {
  memmove(&data_[current_pos_], &object, sizeof(T));
  current_pos_ += sizeof(T);
}

template <typename String>
void Assembly::PackArg(const String& arg, int type) {
  switch(type) {
    case Register:
      Pack((uint64_t)std::stoi(arg.data() + 1));
      break;
    case Ftregister:
      Pack((uint64_t)std::stoi(arg.data() + 3));
      break;
    case Number:
      Pack((int64_t)std::stoi(arg.data()));
      break;
    case Ftnumber:
      Pack(std::stod(arg.data()));
      break;
    case Label:
      Pack((uint64_t)labels_table_[arg]);
      break;
    default:
      std::cerr << "Incorrect args number";
  }
}

