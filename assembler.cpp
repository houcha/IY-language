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

  size_t current_word = 0;

  for (auto& line : text.Lines()) {
    // If word is label, it is first word, so check line[0] only.
    // If word ends in ':', it is label.
    if (IsLabel(line[0])) {
      // ':' is not a part of label name, so remove it.
      std::string_view label_name(line[0].data(), line[0].size() - 1);
      labels_table_[label_name] = current_word;
    }
    current_word += line.size();
  }
}

void Assembly::Parse(FILE* src) {
  Text src_text;
  src_text.Load(src);
  src_text.SplitToLines();

  data_.resize(src_text.CountWords(), 0);

  PreAssembly(src_text);

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
  if (IsLabel(line[0])) { ++current_pos_; return; }
  // Number of command args.
  size_t args_num = line.size() - 1; assert(args_num < 3);

  int left_arg_type = NoArg, right_arg_type = NoArg;
  // Find args types.
  if (args_num >= 1) {
    left_arg_type = GetArgType(line[1]);
  }
  if (args_num == 2) {
    right_arg_type = GetArgType(line[2]);
  }
  // Create command.
  Command command(line[0], left_arg_type, right_arg_type);
  // Push it to data.
  PackCommand(command);

  if (args_num >= 1) {
    // Push left arg to data.
    PackArg(line[1], left_arg_type);
  }
  if (args_num == 2) {
    // Push right arg to data.
    PackArg(line[2], right_arg_type);
  }
}

void Assembly::PackCommand(Command& command) {
  data_[current_pos_++] = reinterpret_cast<value_type&>(command);
}

template <typename String>
void Assembly::PackArg(const String& arg, int type) {
  switch(type) {
    case Register:
      data_[current_pos_++] = std::stoi(arg.data() + 1);
      break;
    case Number:
      data_[current_pos_++] = std::stoi(arg.data());
      break;
    case Label:
      data_[current_pos_++] = labels_table_[arg];
      break;
    default:
      std::cerr << "Incorrect args number";
  }
}

// Local function.
template <typename String>
inline bool IsNumber(const String& arg) {
  for (char letter : arg) {
    if (!('0' <= letter && letter <= '9')) {
      return false;
    }
  }
  return true;
}

template <typename String>
int Assembly::GetArgType(const String& arg) {
  int type = NoArg;
  // Register.
  if (arg[0] == 'r' &&
      (0 <= std::stoi(arg.data() + 1) && std::stoi(arg.data() + 1) <= 15)) {
    type = Register;
  } else
  // Number.
  if (IsNumber(arg)) {
    type = Number;
  } else
  // Label.
  if (labels_table_.count(arg) != 0) {
    type = Label;
  } else {
    std::cerr << "Incorrect args number";
  }

  return type;
}

//=============================== DISASSEMBLY ==================================

static
std::string GenerateLabelName(int pos) {
  return std::string("L_") + std::to_string(pos);
}

template <typename ValueType>
std::string UnpackArg(const ValueType& ch, int arg_type) {
  switch(arg_type) {
    case Register:
      return std::string("r") + std::to_string((int)ch);
    case Number:
      return std::to_string((int)ch);
    case Label:
      return GenerateLabelName((int)ch);
    default:
      std::cerr << "Unknown command: " << ch << "\n";
  }
}

void Disassembly(FILE* src, FILE* dest) {
  AssemblyReader reader;
  reader.Load(src);

  while (!reader.IsFinish()) {
    char lexeme = reader.GetLexeme();
    // If lexeme == 0 then it is label.
    if (lexeme == 0) {
      // -1 because after reading a lexeme we move current_pos_.
      fprintf(dest, "%s:\n",
          GenerateLabelName(reader.GetCurrentPos() - 1).data());
    } else {
      Command command = reinterpret_cast<Command&>(lexeme);

      fprintf(dest, "%s\t", command.GetStringName().data());

      if (command.HasLeftArg()) {
        fprintf(dest, "%s ",
            UnpackArg(reader.GetLexeme(), command.GetLeftArgType()).data());
      }
      if (command.HasRightArg()) {
        fprintf(dest, "%s",
            UnpackArg(reader.GetLexeme(), command.GetRightArgType()).data());
      }
      fputc('\n', dest);
    }
  }
}

