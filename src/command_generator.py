import re


def write_preamble(commands, output):
    output.write("#ifndef COMMANDS_GEN_HPP\n#define COMMANDS_GEN_HPP\n\n")
    output.write("#include <cassert>\n#include \"args.hpp\"\n\n")
    output.write("enum CommandType {\n")
    for command in commands:
        output.write(f"  {command},\n")
    output.write("};\n\n")


def get_arg_types():
    args = []
    with open("src/args.hpp", 'r') as args_file:
        for line in args_file:
            if 'toparse' in line and ',' in line:
                string = line.split(',')[0]
                args.append(string[2 : len(string)])
    return args


def write_command(commands, output):
    arg_types = get_arg_types()
    none_type = arg_types[0]
    output.write("\
/// Command takes 2 bytes.\n\
struct Command {{\n\
  // From 0 to 2 argumants.\n\
  uint8_t left_arg_type : 4;\n\
  uint8_t right_arg_type : 4;\n\
  // One of 256 commands.\n\
  uint8_t type : 8;\n\
\n\
  template <typename Line>\n\
  Command(const Line& line)\n\
    : type(0),\n\
      left_arg_type({}),\n\
      right_arg_type({}) {{\n\n".format(none_type, none_type))

    for command in commands:
        args_pack = commands[command]
        output.write(
        "    if (line[0] == \"{}\") {{\n\
      type = {};\n".format(command, command))
        visited_flag = False
        for args in args_pack:
            visited_flag = True
            if len(args) == 0:
                output.write("      if (line.size() == 1) {\n")
                output.write("        return;\n")
                output.write("      } else\n")
            if len(args) == 1:
                output.write("      if (Is{}Arg(line[1])) {{\n".format(args[0]))
                output.write("\
        left_arg_type = {};\n      }} else\n".format(args[0]))
            if len(args) == 2:
                output.write("      if (Is{}Arg(line[1]) && \
Is{}Arg(line[2])) {{\n".format(args[0], args[1]))
                output.write("\
        left_arg_type = {};\n".format(args[0]))
                output.write("\
        right_arg_type = {};\n      }} else\n".format(args[1]))
        if visited_flag:
            output.write("\
        assert(0 != 0);\n")
        output.write("    } else\n")
    output.write("\
    {\n\
      assert(0 != 0); // Unknown command\n\
    }\n\
  }\n\n")

    sides = ['left', 'right']
    for side in sides:
        output.write("  bool Has{}Arg() {{ return {}_arg_type != {};\
}}\n".format(side.capitalize(), side, none_type))
        output.write("  int Get{}ArgType() {{ return {}_arg_type;\
}}\n".format(side.capitalize(), side))

    output.write("\n  std::string GetStringName() {\n")
    output.write("    switch(type) {\n")
    for command in commands:
        output.write(
"      case {}: return \"{}\";\n".format(command, command))

    output.write("    }\n")
    output.write("  }\n")
    output.write("};\n")


def write_execute_command(commands, output):
    arg_types = get_arg_types()
    none_type = arg_types[0]
    output.write("\
void ExecuteCommand(Command& command, uint64_t left_arg, uint64_t right_arg) {\n\
  switch (command.type) {\n")
    for command in commands:
        output.write("    case CommandType::{}:\n".format(command))
        args_pack = commands[command]
        for args in args_pack:
            if len(args) == 0:
                output.write("\
      if (command.GetLeftArgType() == {} &&\n\
          command.GetRightArgType() == {})\n".format(none_type, none_type))
                output.write("\
        {}();\n\
      else\n".format(command))
            if len(args) == 1:
                output.write("\
      if (command.GetLeftArgType() == {} &&\n\
          command.GetRightArgType() == {})\n".format(args[0], none_type))
                output.write("\
        {}(reinterpret_cast<{}&>(left_arg));\n\
      else\n".format(command, args[0].lower() + "_type"))
            if len(args) == 2:
                output.write("\
      if (command.GetLeftArgType() == {} &&\n\
          command.GetRightArgType() == {})\n".format(args[0], args[1]))
                output.write("\
        {}(reinterpret_cast<{}&>(left_arg), reinterpret_cast<{}&>(right_arg));\n\
      else\n".format(command, args[0].lower() + '_type', args[1].lower() +
          '_type'))
        output.write("\
        assert(0 != 0);\n")
        output.write("\
      break;\n")
    output.write("  }\n")
    output.write("}\n")


def add_command(commands, line):
    command = line[5 : line.find('(')]
    full_args = line[line.find('(') + 1 : line.find(')')].split(", ")
    args = []
    for arg in full_args:
        args.append(arg.split(' ')[0])
    if command not in commands:
        commands[command] = []
    args = [x for x in args if x != ""]
    commands[command].append(args)


def convert_to_enum(commands):
    for args_pack in commands.values():
        for args in args_pack:
            pos = 0
            for arg in args:
                args[pos] = arg.split('_')[0].capitalize()
                pos += 1


def fill_commands(commands, input_file):
    for line in input_file:
        if 'void' in line and '(' in line and ')' in line:
            add_command(commands, line)


if __name__ == "__main__":
    commands = dict()
    with open("src/processor_commands.inl", 'r') as input_file:
        fill_commands(commands, input_file)
    convert_to_enum(commands)
    with open("src/command.gen.hpp", 'w') as output_file:
        write_preamble(commands, output_file)
        write_command(commands, output_file)
        output_file.write("\n#endif // COMMANDS_GEN_HPP\n")
        output_file.write('\n')
    with open("src/processor_execute_command.gen.inl", 'w') as output_file:
        write_execute_command(commands, output_file)
        output_file.write('\n')

