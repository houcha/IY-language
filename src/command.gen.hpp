#ifndef COMMANDS_GEN_HPP
#define COMMANDS_GEN_HPP

#include <cassert>
#include "args.hpp"

enum CommandType {
  push,
  pop,
  mov,
  movf,
  in,
  fin,
  out,
  fout,
  cmp,
  cmpf,
  add,
  addf,
  sub,
  subf,
  mul,
  mulf,
  divf,
  fsqrt,
  lnot,
  flnot,
  finf,
  jmp,
  jz,
  jnz,
  jl,
  jg,
  jle,
  jge,
  call,
  ret,
  end,
};

/// Command takes 2 bytes.
struct Command {
  // From 0 to 2 argumants.
  uint8_t left_arg_type : 4;
  uint8_t right_arg_type : 4;
  // One of 256 commands.
  uint8_t type : 8;

  template <typename Line>
  Command(const Line& line)
    : type(0),
      left_arg_type(NoneArg),
      right_arg_type(NoneArg) {

    if (line[0] == "push") {
      type = push;
      if (IsNumberArg(line[1])) {
        left_arg_type = Number;
      } else
      if (IsRegisterArg(line[1])) {
        left_arg_type = Register;
      } else
        assert(0 != 0);
    } else
    if (line[0] == "pop") {
      type = pop;
      if (IsRegisterArg(line[1])) {
        left_arg_type = Register;
      } else
        assert(0 != 0);
    } else
    if (line[0] == "mov") {
      type = mov;
      if (IsRegisterArg(line[1]) && IsNumberArg(line[2])) {
        left_arg_type = Register;
        right_arg_type = Number;
      } else
      if (IsRegisterArg(line[1]) && IsRegisterArg(line[2])) {
        left_arg_type = Register;
        right_arg_type = Register;
      } else
        assert(0 != 0);
    } else
    if (line[0] == "movf") {
      type = movf;
      if (IsFtregisterArg(line[1]) && IsFtnumberArg(line[2])) {
        left_arg_type = Ftregister;
        right_arg_type = Ftnumber;
      } else
      if (IsFtregisterArg(line[1]) && IsFtregisterArg(line[2])) {
        left_arg_type = Ftregister;
        right_arg_type = Ftregister;
      } else
        assert(0 != 0);
    } else
    if (line[0] == "in") {
      type = in;
      if (line.size() == 1) {
        return;
      } else
      if (IsRegisterArg(line[1])) {
        left_arg_type = Register;
      } else
        assert(0 != 0);
    } else
    if (line[0] == "fin") {
      type = fin;
      if (IsFtregisterArg(line[1])) {
        left_arg_type = Ftregister;
      } else
        assert(0 != 0);
    } else
    if (line[0] == "out") {
      type = out;
      if (line.size() == 1) {
        return;
      } else
      if (IsRegisterArg(line[1])) {
        left_arg_type = Register;
      } else
        assert(0 != 0);
    } else
    if (line[0] == "fout") {
      type = fout;
      if (IsFtregisterArg(line[1])) {
        left_arg_type = Ftregister;
      } else
        assert(0 != 0);
    } else
    if (line[0] == "cmp") {
      type = cmp;
      if (IsNumberArg(line[1]) && IsNumberArg(line[2])) {
        left_arg_type = Number;
        right_arg_type = Number;
      } else
      if (IsRegisterArg(line[1]) && IsNumberArg(line[2])) {
        left_arg_type = Register;
        right_arg_type = Number;
      } else
      if (IsRegisterArg(line[1]) && IsRegisterArg(line[2])) {
        left_arg_type = Register;
        right_arg_type = Register;
      } else
        assert(0 != 0);
    } else
    if (line[0] == "cmpf") {
      type = cmpf;
      if (IsFtnumberArg(line[1]) && IsFtnumberArg(line[2])) {
        left_arg_type = Ftnumber;
        right_arg_type = Ftnumber;
      } else
      if (IsFtregisterArg(line[1]) && IsFtnumberArg(line[2])) {
        left_arg_type = Ftregister;
        right_arg_type = Ftnumber;
      } else
      if (IsFtregisterArg(line[1]) && IsFtregisterArg(line[2])) {
        left_arg_type = Ftregister;
        right_arg_type = Ftregister;
      } else
        assert(0 != 0);
    } else
    if (line[0] == "add") {
      type = add;
      if (IsRegisterArg(line[1]) && IsNumberArg(line[2])) {
        left_arg_type = Register;
        right_arg_type = Number;
      } else
      if (IsRegisterArg(line[1]) && IsRegisterArg(line[2])) {
        left_arg_type = Register;
        right_arg_type = Register;
      } else
        assert(0 != 0);
    } else
    if (line[0] == "addf") {
      type = addf;
      if (IsFtregisterArg(line[1]) && IsFtnumberArg(line[2])) {
        left_arg_type = Ftregister;
        right_arg_type = Ftnumber;
      } else
      if (IsFtregisterArg(line[1]) && IsFtregisterArg(line[2])) {
        left_arg_type = Ftregister;
        right_arg_type = Ftregister;
      } else
        assert(0 != 0);
    } else
    if (line[0] == "sub") {
      type = sub;
      if (IsRegisterArg(line[1]) && IsNumberArg(line[2])) {
        left_arg_type = Register;
        right_arg_type = Number;
      } else
      if (IsRegisterArg(line[1]) && IsRegisterArg(line[2])) {
        left_arg_type = Register;
        right_arg_type = Register;
      } else
        assert(0 != 0);
    } else
    if (line[0] == "subf") {
      type = subf;
      if (IsFtregisterArg(line[1]) && IsFtnumberArg(line[2])) {
        left_arg_type = Ftregister;
        right_arg_type = Ftnumber;
      } else
      if (IsFtregisterArg(line[1]) && IsFtregisterArg(line[2])) {
        left_arg_type = Ftregister;
        right_arg_type = Ftregister;
      } else
        assert(0 != 0);
    } else
    if (line[0] == "mul") {
      type = mul;
      if (IsRegisterArg(line[1]) && IsNumberArg(line[2])) {
        left_arg_type = Register;
        right_arg_type = Number;
      } else
      if (IsRegisterArg(line[1]) && IsRegisterArg(line[2])) {
        left_arg_type = Register;
        right_arg_type = Register;
      } else
        assert(0 != 0);
    } else
    if (line[0] == "mulf") {
      type = mulf;
      if (IsFtregisterArg(line[1]) && IsFtnumberArg(line[2])) {
        left_arg_type = Ftregister;
        right_arg_type = Ftnumber;
      } else
      if (IsFtregisterArg(line[1]) && IsFtregisterArg(line[2])) {
        left_arg_type = Ftregister;
        right_arg_type = Ftregister;
      } else
        assert(0 != 0);
    } else
    if (line[0] == "divf") {
      type = divf;
      if (IsFtregisterArg(line[1]) && IsFtnumberArg(line[2])) {
        left_arg_type = Ftregister;
        right_arg_type = Ftnumber;
      } else
      if (IsFtregisterArg(line[1]) && IsFtregisterArg(line[2])) {
        left_arg_type = Ftregister;
        right_arg_type = Ftregister;
      } else
        assert(0 != 0);
    } else
    if (line[0] == "fsqrt") {
      type = fsqrt;
      if (IsFtregisterArg(line[1])) {
        left_arg_type = Ftregister;
      } else
        assert(0 != 0);
    } else
    if (line[0] == "lnot") {
      type = lnot;
      if (IsRegisterArg(line[1])) {
        left_arg_type = Register;
      } else
        assert(0 != 0);
    } else
    if (line[0] == "flnot") {
      type = flnot;
      if (IsFtregisterArg(line[1])) {
        left_arg_type = Ftregister;
      } else
        assert(0 != 0);
    } else
    if (line[0] == "finf") {
      type = finf;
      if (IsFtregisterArg(line[1])) {
        left_arg_type = Ftregister;
      } else
        assert(0 != 0);
    } else
    if (line[0] == "jmp") {
      type = jmp;
      if (IsLabelArg(line[1])) {
        left_arg_type = Label;
      } else
        assert(0 != 0);
    } else
    if (line[0] == "jz") {
      type = jz;
      if (IsLabelArg(line[1])) {
        left_arg_type = Label;
      } else
        assert(0 != 0);
    } else
    if (line[0] == "jnz") {
      type = jnz;
      if (IsLabelArg(line[1])) {
        left_arg_type = Label;
      } else
        assert(0 != 0);
    } else
    if (line[0] == "jl") {
      type = jl;
      if (IsLabelArg(line[1])) {
        left_arg_type = Label;
      } else
        assert(0 != 0);
    } else
    if (line[0] == "jg") {
      type = jg;
      if (IsLabelArg(line[1])) {
        left_arg_type = Label;
      } else
        assert(0 != 0);
    } else
    if (line[0] == "jle") {
      type = jle;
      if (IsLabelArg(line[1])) {
        left_arg_type = Label;
      } else
        assert(0 != 0);
    } else
    if (line[0] == "jge") {
      type = jge;
      if (IsLabelArg(line[1])) {
        left_arg_type = Label;
      } else
        assert(0 != 0);
    } else
    if (line[0] == "call") {
      type = call;
      if (IsLabelArg(line[1])) {
        left_arg_type = Label;
      } else
        assert(0 != 0);
    } else
    if (line[0] == "ret") {
      type = ret;
      if (line.size() == 1) {
        return;
      } else
        assert(0 != 0);
    } else
    if (line[0] == "end") {
      type = end;
      if (line.size() == 1) {
        return;
      } else
        assert(0 != 0);
    } else
    {
      assert(0 != 0); // Unknown command
    }
  }

  bool HasLeftArg() { return left_arg_type != NoneArg;}
  int GetLeftArgType() { return left_arg_type;}
  bool HasRightArg() { return right_arg_type != NoneArg;}
  int GetRightArgType() { return right_arg_type;}

  std::string GetStringName() {
    switch(type) {
      case push: return "push";
      case pop: return "pop";
      case mov: return "mov";
      case movf: return "movf";
      case in: return "in";
      case fin: return "fin";
      case out: return "out";
      case fout: return "fout";
      case cmp: return "cmp";
      case cmpf: return "cmpf";
      case add: return "add";
      case addf: return "addf";
      case sub: return "sub";
      case subf: return "subf";
      case mul: return "mul";
      case mulf: return "mulf";
      case divf: return "divf";
      case fsqrt: return "fsqrt";
      case lnot: return "lnot";
      case flnot: return "flnot";
      case finf: return "finf";
      case jmp: return "jmp";
      case jz: return "jz";
      case jnz: return "jnz";
      case jl: return "jl";
      case jg: return "jg";
      case jle: return "jle";
      case jge: return "jge";
      case call: return "call";
      case ret: return "ret";
      case end: return "end";
    }
  }
};

#endif // COMMANDS_GEN_HPP

