#include "disassembler.hpp"


std::string GenerateLabelName(int pos) {
  return std::string("L_") + std::to_string(pos);
}

std::string UnpackArg(uint64_t arg, int arg_type) {
  switch(arg_type) {
    case NoneArg:
      return "None";
    case Register:
      return std::string("r") + std::to_string(arg);
    case Ftregister:
      return std::string("xmm") + std::to_string(arg);
    case Number:
      return std::to_string(reinterpret_cast<int64_t&>(arg));
    case Ftnumber:
      return std::to_string(reinterpret_cast<double&>(arg));
    case Label:
      return GenerateLabelName(arg);
    default:
      std::cerr << "Unknown command: " << arg << "\n";
  }
}

void Disassembly(FILE* src, FILE* dest) {
  Text src_text;
  src_text.Load(src);

  char* data = src_text.Data();
  char* data_end = data + src_text.Size();
  char* pos = data;

  while (pos != data_end) {
    Command command = GetLexeme<Command>(pos);
    // If command is 0 then it is label.
    if (reinterpret_cast<uint16_t&>(command) == 0) {
      // -sizeof(Command) because after reading a lexeme we move current_pos_.
      fprintf(dest, "%s:\n",
          GenerateLabelName(pos - data - sizeof(Command)).data());
    } else {

      fprintf(dest, "%s\t", command.GetStringName().data());

      if (command.HasLeftArg()) {
        fprintf(dest, "%s ",
            UnpackArg(
              GetLexeme<uint64_t>(pos), command.GetLeftArgType()).data());
      }
      if (command.HasRightArg()) {
        fprintf(dest, "%s",
            UnpackArg(
              GetLexeme<uint64_t>(pos), command.GetRightArgType()).data());
      }
      fputc('\n', dest);
    }
  }
}

