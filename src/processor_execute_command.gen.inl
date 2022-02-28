void ExecuteCommand(Command& command, uint64_t left_arg, uint64_t right_arg) {
  switch (command.type) {
    case CommandType::push:
      if (command.GetLeftArgType() == Number &&
          command.GetRightArgType() == NoneArg)
        push(reinterpret_cast<number_type&>(left_arg));
      else
      if (command.GetLeftArgType() == Register &&
          command.GetRightArgType() == NoneArg)
        push(reinterpret_cast<register_type&>(left_arg));
      else
        assert(0 != 0);
      break;
    case CommandType::pop:
      if (command.GetLeftArgType() == Register &&
          command.GetRightArgType() == NoneArg)
        pop(reinterpret_cast<register_type&>(left_arg));
      else
        assert(0 != 0);
      break;
    case CommandType::mov:
      if (command.GetLeftArgType() == Register &&
          command.GetRightArgType() == Number)
        mov(reinterpret_cast<register_type&>(left_arg), reinterpret_cast<number_type&>(right_arg));
      else
      if (command.GetLeftArgType() == Register &&
          command.GetRightArgType() == Register)
        mov(reinterpret_cast<register_type&>(left_arg), reinterpret_cast<register_type&>(right_arg));
      else
        assert(0 != 0);
      break;
    case CommandType::movf:
      if (command.GetLeftArgType() == Ftregister &&
          command.GetRightArgType() == Ftnumber)
        movf(reinterpret_cast<ftregister_type&>(left_arg), reinterpret_cast<ftnumber_type&>(right_arg));
      else
      if (command.GetLeftArgType() == Ftregister &&
          command.GetRightArgType() == Ftregister)
        movf(reinterpret_cast<ftregister_type&>(left_arg), reinterpret_cast<ftregister_type&>(right_arg));
      else
        assert(0 != 0);
      break;
    case CommandType::in:
      if (command.GetLeftArgType() == NoneArg &&
          command.GetRightArgType() == NoneArg)
        in();
      else
      if (command.GetLeftArgType() == Register &&
          command.GetRightArgType() == NoneArg)
        in(reinterpret_cast<register_type&>(left_arg));
      else
        assert(0 != 0);
      break;
    case CommandType::fin:
      if (command.GetLeftArgType() == Ftregister &&
          command.GetRightArgType() == NoneArg)
        fin(reinterpret_cast<ftregister_type&>(left_arg));
      else
        assert(0 != 0);
      break;
    case CommandType::out:
      if (command.GetLeftArgType() == NoneArg &&
          command.GetRightArgType() == NoneArg)
        out();
      else
      if (command.GetLeftArgType() == Register &&
          command.GetRightArgType() == NoneArg)
        out(reinterpret_cast<register_type&>(left_arg));
      else
        assert(0 != 0);
      break;
    case CommandType::fout:
      if (command.GetLeftArgType() == Ftregister &&
          command.GetRightArgType() == NoneArg)
        fout(reinterpret_cast<ftregister_type&>(left_arg));
      else
        assert(0 != 0);
      break;
    case CommandType::cmp:
      if (command.GetLeftArgType() == Number &&
          command.GetRightArgType() == Number)
        cmp(reinterpret_cast<number_type&>(left_arg), reinterpret_cast<number_type&>(right_arg));
      else
      if (command.GetLeftArgType() == Register &&
          command.GetRightArgType() == Number)
        cmp(reinterpret_cast<register_type&>(left_arg), reinterpret_cast<number_type&>(right_arg));
      else
      if (command.GetLeftArgType() == Register &&
          command.GetRightArgType() == Register)
        cmp(reinterpret_cast<register_type&>(left_arg), reinterpret_cast<register_type&>(right_arg));
      else
        assert(0 != 0);
      break;
    case CommandType::cmpf:
      if (command.GetLeftArgType() == Ftnumber &&
          command.GetRightArgType() == Ftnumber)
        cmpf(reinterpret_cast<ftnumber_type&>(left_arg), reinterpret_cast<ftnumber_type&>(right_arg));
      else
      if (command.GetLeftArgType() == Ftregister &&
          command.GetRightArgType() == Ftnumber)
        cmpf(reinterpret_cast<ftregister_type&>(left_arg), reinterpret_cast<ftnumber_type&>(right_arg));
      else
      if (command.GetLeftArgType() == Ftregister &&
          command.GetRightArgType() == Ftregister)
        cmpf(reinterpret_cast<ftregister_type&>(left_arg), reinterpret_cast<ftregister_type&>(right_arg));
      else
        assert(0 != 0);
      break;
    case CommandType::add:
      if (command.GetLeftArgType() == Register &&
          command.GetRightArgType() == Number)
        add(reinterpret_cast<register_type&>(left_arg), reinterpret_cast<number_type&>(right_arg));
      else
      if (command.GetLeftArgType() == Register &&
          command.GetRightArgType() == Register)
        add(reinterpret_cast<register_type&>(left_arg), reinterpret_cast<register_type&>(right_arg));
      else
        assert(0 != 0);
      break;
    case CommandType::addf:
      if (command.GetLeftArgType() == Ftregister &&
          command.GetRightArgType() == Ftnumber)
        addf(reinterpret_cast<ftregister_type&>(left_arg), reinterpret_cast<ftnumber_type&>(right_arg));
      else
      if (command.GetLeftArgType() == Ftregister &&
          command.GetRightArgType() == Ftregister)
        addf(reinterpret_cast<ftregister_type&>(left_arg), reinterpret_cast<ftregister_type&>(right_arg));
      else
        assert(0 != 0);
      break;
    case CommandType::sub:
      if (command.GetLeftArgType() == Register &&
          command.GetRightArgType() == Number)
        sub(reinterpret_cast<register_type&>(left_arg), reinterpret_cast<number_type&>(right_arg));
      else
      if (command.GetLeftArgType() == Register &&
          command.GetRightArgType() == Register)
        sub(reinterpret_cast<register_type&>(left_arg), reinterpret_cast<register_type&>(right_arg));
      else
        assert(0 != 0);
      break;
    case CommandType::subf:
      if (command.GetLeftArgType() == Ftregister &&
          command.GetRightArgType() == Ftnumber)
        subf(reinterpret_cast<ftregister_type&>(left_arg), reinterpret_cast<ftnumber_type&>(right_arg));
      else
      if (command.GetLeftArgType() == Ftregister &&
          command.GetRightArgType() == Ftregister)
        subf(reinterpret_cast<ftregister_type&>(left_arg), reinterpret_cast<ftregister_type&>(right_arg));
      else
        assert(0 != 0);
      break;
    case CommandType::mul:
      if (command.GetLeftArgType() == Register &&
          command.GetRightArgType() == Number)
        mul(reinterpret_cast<register_type&>(left_arg), reinterpret_cast<number_type&>(right_arg));
      else
      if (command.GetLeftArgType() == Register &&
          command.GetRightArgType() == Register)
        mul(reinterpret_cast<register_type&>(left_arg), reinterpret_cast<register_type&>(right_arg));
      else
        assert(0 != 0);
      break;
    case CommandType::mulf:
      if (command.GetLeftArgType() == Ftregister &&
          command.GetRightArgType() == Ftnumber)
        mulf(reinterpret_cast<ftregister_type&>(left_arg), reinterpret_cast<ftnumber_type&>(right_arg));
      else
      if (command.GetLeftArgType() == Ftregister &&
          command.GetRightArgType() == Ftregister)
        mulf(reinterpret_cast<ftregister_type&>(left_arg), reinterpret_cast<ftregister_type&>(right_arg));
      else
        assert(0 != 0);
      break;
    case CommandType::divf:
      if (command.GetLeftArgType() == Ftregister &&
          command.GetRightArgType() == Ftnumber)
        divf(reinterpret_cast<ftregister_type&>(left_arg), reinterpret_cast<ftnumber_type&>(right_arg));
      else
      if (command.GetLeftArgType() == Ftregister &&
          command.GetRightArgType() == Ftregister)
        divf(reinterpret_cast<ftregister_type&>(left_arg), reinterpret_cast<ftregister_type&>(right_arg));
      else
        assert(0 != 0);
      break;
    case CommandType::fsqrt:
      if (command.GetLeftArgType() == Ftregister &&
          command.GetRightArgType() == NoneArg)
        fsqrt(reinterpret_cast<ftregister_type&>(left_arg));
      else
        assert(0 != 0);
      break;
    case CommandType::lnot:
      if (command.GetLeftArgType() == Register &&
          command.GetRightArgType() == NoneArg)
        lnot(reinterpret_cast<register_type&>(left_arg));
      else
        assert(0 != 0);
      break;
    case CommandType::flnot:
      if (command.GetLeftArgType() == Ftregister &&
          command.GetRightArgType() == NoneArg)
        flnot(reinterpret_cast<ftregister_type&>(left_arg));
      else
        assert(0 != 0);
      break;
    case CommandType::finf:
      if (command.GetLeftArgType() == Ftregister &&
          command.GetRightArgType() == NoneArg)
        finf(reinterpret_cast<ftregister_type&>(left_arg));
      else
        assert(0 != 0);
      break;
    case CommandType::jmp:
      if (command.GetLeftArgType() == Label &&
          command.GetRightArgType() == NoneArg)
        jmp(reinterpret_cast<label_type&>(left_arg));
      else
        assert(0 != 0);
      break;
    case CommandType::jz:
      if (command.GetLeftArgType() == Label &&
          command.GetRightArgType() == NoneArg)
        jz(reinterpret_cast<label_type&>(left_arg));
      else
        assert(0 != 0);
      break;
    case CommandType::jnz:
      if (command.GetLeftArgType() == Label &&
          command.GetRightArgType() == NoneArg)
        jnz(reinterpret_cast<label_type&>(left_arg));
      else
        assert(0 != 0);
      break;
    case CommandType::jl:
      if (command.GetLeftArgType() == Label &&
          command.GetRightArgType() == NoneArg)
        jl(reinterpret_cast<label_type&>(left_arg));
      else
        assert(0 != 0);
      break;
    case CommandType::jg:
      if (command.GetLeftArgType() == Label &&
          command.GetRightArgType() == NoneArg)
        jg(reinterpret_cast<label_type&>(left_arg));
      else
        assert(0 != 0);
      break;
    case CommandType::jle:
      if (command.GetLeftArgType() == Label &&
          command.GetRightArgType() == NoneArg)
        jle(reinterpret_cast<label_type&>(left_arg));
      else
        assert(0 != 0);
      break;
    case CommandType::jge:
      if (command.GetLeftArgType() == Label &&
          command.GetRightArgType() == NoneArg)
        jge(reinterpret_cast<label_type&>(left_arg));
      else
        assert(0 != 0);
      break;
    case CommandType::call:
      if (command.GetLeftArgType() == Label &&
          command.GetRightArgType() == NoneArg)
        call(reinterpret_cast<label_type&>(left_arg));
      else
        assert(0 != 0);
      break;
    case CommandType::ret:
      if (command.GetLeftArgType() == NoneArg &&
          command.GetRightArgType() == NoneArg)
        ret();
      else
        assert(0 != 0);
      break;
    case CommandType::end:
      if (command.GetLeftArgType() == NoneArg &&
          command.GetRightArgType() == NoneArg)
        end();
      else
        assert(0 != 0);
      break;
  }
}

