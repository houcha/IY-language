#include "processor.hpp"


void Run(FILE* assembled) {
  Processor processor;

  AssemblyReader reader;
  reader.Load(assembled);

  while (!reader.IsFinish()) {
    char lexeme = reader.GetLexeme();
    if (lexeme != 0) {
      Command& command = reinterpret_cast<Command&>(lexeme);

      int8_t left_arg = NoArg, right_arg = NoArg;
      if (command.HasLeftArg()) left_arg = reader.GetLexeme();
      if (command.HasRightArg()) right_arg = reader.GetLexeme();

      processor.ExecuteCommand(command, left_arg, right_arg);
    }
  }
}

/* I see two ways:
 * 1. Define commands as functions (as it is below).
 * Cons: code becomes more fregmented.
 * 2. Make a new class for every command with overridden Execute() method.
 * Cons: virtual inheritance is slow.
 */

/// ExecuteCommand is processor method because it uses stack,
/// memory and registers, which are processor fields.
int Processor::ExecuteCommand(const Command& command,
                              int8_t left_arg, int8_t right_arg,
                              const AssemblyReader& reader) {

  switch(command.GetType()) {
//------------------------------------------------------------------------------
    // mov reg reg
    // mov reg num
    case mov: {
      // If right arg is reg, then right_arg is reg number, so convert it to
      // very reg.
      if (command.GetRightArgType() == Register) {
        right_arg = r[right_arg];
      }
      r[left_arg] = right_arg;
      break;
    }
//------------------------------------------------------------------------------
    // push num
    // push reg
    case push: {
      // If left arg is reg, then left_arg is reg number, so convert it to
      // very reg.
      if (command.GetLeftArgType() == Register) {
        left_arg = r[left_arg];
      }
      stack_.push(left_arg);
      break;
    }
//------------------------------------------------------------------------------
    // pop
    // pop reg
    case pop: {
      if (command.GetLeftArgType() == Register) {
        r[left_arg] = stack_.top();
      }
      stack_.pop();
      break;
    }
//------------------------------------------------------------------------------
    // in
    case in: {
      int value = 0;
      scanf("%d", &value);
      stack_.push(value);
      break;
    }
//------------------------------------------------------------------------------
    // out
    case out: {
      printf("%d", stack_.top());
      stack_.pop();
      break;
    }
//------------------------------------------------------------------------------
    // add
    // add reg reg
    // add reg num
    case add: {
      if (command.GetLeftArgType() == NoArg) {
        int first = stack_.top();
        stack_.pop();
        stack_.top() += first;
      } else {
        if (command.GetRightArgType() == Register) {
          right_arg = r[right_arg];
        }
        r[left_arg] += right_arg;
      }
      break;
    }
//------------------------------------------------------------------------------
    // sub
    // sub reg reg
    // sub reg num
    case sub: {
      if (command.GetLeftArgType() == NoArg) {
        int first = stack_.top();
        stack_.pop();
        stack_.top() -= first;
      } else {
        if (command.GetRightArgType() == Register) {
          right_arg = r[right_arg];
        }
        r[left_arg] -= right_arg;
      }
      break;
    }
//------------------------------------------------------------------------------
    // mul
    // mul reg reg
    // mul reg num
    case mul: {
      if (command.GetLeftArgType() == NoArg) {
        int first = stack_.top();
        stack_.pop();
        stack_.top() *= first;
      } else {
        if (command.GetRightArgType() == Register) {
          right_arg = r[right_arg];
        }
        r[left_arg] *= right_arg;
      }
      break;
    }
//------------------------------------------------------------------------------
    // cmp reg reg
    // cmp reg num
    // cmp num num
    case cmp: {
      if (command.GetLeftArgType() == Register) {
        left_arg = r[left_arg];
      }
      if (command.GetRightArgType() == Register) {
        right_arg = r[right_arg];
      }

      int result = left_arg - right_arg;
      if (result < 0) {
        SF = true;
      } else
      if (result > 0) {
        SF = false;
      } else {
        ZF = true;
      }
      break;
    }
//------------------------------------------------------------------------------
    // jmp label
    case jmp: {
      reader.GoTo(left_arg);
      break;
    }
  }
}

