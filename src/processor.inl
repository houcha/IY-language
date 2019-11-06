void Run(FILE* assembled, FILE* listing) {
  Processor processor;
  processor.LoadProgram(assembled);
  processor.Execute(listing);
}


template <int FullMemorySize, int StackSize, typename StackValueType>
Processor<FullMemorySize, StackSize, StackValueType>::Processor()
    : memory((char*)malloc(FullMemorySize * sizeof(char))),
      data(nullptr),
      text(nullptr),
      ram(nullptr),
      stack_frame(nullptr),
      sp(nullptr),
      pc(nullptr),
      r(),
      xmm(),
      ZF(0),
      SF(0),
      CF(0),
      OF(0) {

  data = memory;
  text = data + 0;
  stack_frame = memory + (FullMemorySize - StackSize) - 1;
  sp = memory + FullMemorySize - 1;
}

template <int FullMemorySize, int StackSize, typename StackValueType>
Processor<FullMemorySize, StackSize, StackValueType>::~Processor() {
  free(memory);
}

template <int FullMemorySize, int StackSize, typename StackValueType>
void Processor<FullMemorySize, StackSize, StackValueType>::
LoadProgram(FILE* assembled) {
  size_t text_size = GetFileSize(assembled);
  fread(text, sizeof(char), text_size, assembled);

  ram = text + text_size;
  pc = text;
}

template <int FullMemorySize, int StackSize, typename StackValueType>
void Processor<FullMemorySize, StackSize, StackValueType>::
Execute(FILE* listing) {

  if (listing != nullptr) {
    fprintf(listing, "Binary\t\t\tString\n");
    fprintf(listing,
        "Shift\tLeft\tRight\tCommand\tLeft\tRight\n\n");
  }

  while (pc != ram) {
    Command command = GetLexeme<Command>(pc);
    // If command is 0, then it is label, so skip it.
    if (reinterpret_cast<int16_t&>(command) == 0) { continue; }

    uint64_t left_arg = 0, right_arg = 0;
    if (command.HasLeftArg()) left_arg = GetLexeme<uint64_t>(pc);
    if (command.HasRightArg()) right_arg = GetLexeme<uint64_t>(pc);

    ExecuteCommand(command, left_arg, right_arg);

    if (listing != nullptr) {
      PrintToListing(listing, command, left_arg, right_arg, pc);
    }
  }
}

template <int FullMemorySize, int StackSize, typename StackValueType>
void Processor<FullMemorySize, StackSize, StackValueType>::
PrintToListing(FILE* listing, Command& command,
               uint64_t left_arg, uint64_t right_arg, char* pc) {

  fprintf(listing, "+%ld\t", pc - text);
  PrintArgValue(listing, left_arg, command.GetLeftArgType());
  PrintArgValue(listing, right_arg, command.GetRightArgType());

  fprintf(listing, "%s\t", command.GetStringName().data());
  fprintf(listing, "%s\t",
      UnpackArg(left_arg, command.GetLeftArgType()).data());
  fprintf(listing, "%s\n",
      UnpackArg(right_arg, command.GetRightArgType()).data());
}

template <int FullMemorySize, int StackSize, typename StackValueType>
void Processor<FullMemorySize, StackSize, StackValueType>::
PrintArgValue(FILE* listing, uint64_t arg, int arg_type) {
  switch(arg_type) {
    case NoneArg:
      fprintf(listing, "\t");
      break;
    case Register:
      fprintf(listing, "%ld\t", r[arg]);
      break;
    case Ftregister:
      fprintf(listing, "%.3f\t", xmm[arg]);
      break;
    case Number:
      fprintf(listing, "%ld\t", reinterpret_cast<int64_t&>(arg));
      break;
    case Ftnumber:
      fprintf(listing, "%.3f\t", reinterpret_cast<double&>(arg));
      break;
    case Label:
      fprintf(listing, "%ld\t", arg);
      break;
  }
}

