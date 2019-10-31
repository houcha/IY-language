#ifndef COMMAND_HPP
#define COMMAND_HPP

enum CommandType {
  mov, push, pop,
  in, out,
  add, sub, mul,
  cmp,
  jmp, jz, jnz,
};

enum ArgType {
  NoArg, Register, Number, Label,
};


/// Command takes 1 byte.
class Command {

  private:

    // From 0 to 2 argumants.
    uint8_t left_arg_type_ : 2;
    uint8_t right_arg_type_ : 2;
    uint8_t type_ : 4;

  public:

    template <typename String>
    Command(const String& word, int LeftArgType, int RightArgType)
        : left_arg_type_(LeftArgType),
          right_arg_type_(RightArgType) {

      if (word == "push") {
        type_ = push;
      } else
      if (word == "mov") {
        type_ = mov;
      } else
      if (word == "pop") {
        type_ = pop;
      } else
      if (word == "in") {
        type_ = in;
      } else
      if (word == "out") {
        type_ = out;
      } else
      if (word == "add") {
        type_ = add;
      } else
      if (word == "sub") {
        type_ = sub;
      } else
      if (word == "mul") {
        type_ = mul;
      } else
      if (word == "cmp") {
        type_ = cmp;
      } else
      if (word == "jmp") {
        type_ = jmp;
      } else
      if (word == "jz") {
        type_ = jz;
      } else
      if (word == "jnz") {
        type_ = jnz;
      } else {
        std::cerr << "Unknown command: " << word << "\n";
      }
    }

    std::string GetStringName() {
      switch(type_) {
        case push: return "push";
        case mov:  return "mov";
        case pop:  return "pop";
        case in:   return "in";
        case out:  return "out";
        case add:  return "add";
        case sub:  return "sub";
        case mul:  return "mul";
        case cmp:  return "cmp";
        case jmp:  return "jmp";
        case jz:   return "jz";
        case jnz:  return "jnz";
        default:
          std::cerr << "Unknown command\n";
      }
    }

    bool HasLeftArg()  { return left_arg_type_  != NoArg; }
    bool HasRightArg() { return right_arg_type_ != NoArg; }
    int  GetLeftArgType()  { return left_arg_type_;  }
    int  GetRightArgType() { return right_arg_type_; }

    int GetType() { return type_; }

    size_t GetArgsNum() { return HasLeftArg() + HasRightArg(); }
};


#endif // COMMAND_HPP

