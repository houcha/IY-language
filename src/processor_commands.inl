//================================= PUSH =======================================
void push(number_type num) {
  sp -= sizeof(StackValueType);
  *(StackValueType*)sp = num;
}

void push(register_type reg) {
  push(r[reg]);
}

//================================= POP ========================================
void pop(register_type reg) {
  r[reg] = *(StackValueType*)sp;
  sp += sizeof(StackValueType);
}

//================================= MOV ========================================
void mov(register_type reg, number_type num) {
  r[reg] = num;
}

void mov(register_type reg1, register_type reg2) {
  mov(reg1, r[reg2]);
}

void fmov(ftregister_type freg, ftnumber_type num) {
  xmm[freg] = num;
}

void fmov(ftregister_type freg1, ftregister_type freg2) {
  fmov(freg1, xmm[freg2]);
}

//================================= IN =========================================
void in() {
  StackValueType value = 0;
  scanf("%lu", &value);
  push((number_type)value);
}

void in(register_type reg) {
  scanf("%ld", &r[reg]);
}

void fin(ftregister_type freg) {
  scanf("%lf", &xmm[freg]);
}

//================================= OUT ========================================
void out() {
  printf("%ld\n", *(StackValueType*)sp);
  sp += sizeof(StackValueType);
}

void out(register_type reg) {
  printf("%ld\n", r[reg]);
}

void fout(ftregister_type freg) {
  printf("%.2f\n", xmm[freg]);
}

//================================= CMP ========================================
void cmp(number_type num1, number_type num2) {
  num1 -= num2;
  ZF = 0;
  SF = 0;
  CF = 0;
  OF = 0;
  if (num1 == 0) {
    ZF = 1;
  } else
  if (num1 < 0) {
    SF = 1;
  }
}

void cmp(register_type reg, number_type num) {
  cmp(r[reg], num);
}

void cmp(register_type reg1, register_type reg2) {
  cmp(r[reg1], r[reg2]);
}

void fcmp(ftnumber_type num1, ftnumber_type num2) {
  num1 -= num2;
  ZF = 0;
  SF = 0;
  CF = 0;
  OF = 0;
  if (num1 == 0) {
    ZF = 1;
  } else
  if (num1 < 0) {
    SF = 1;
  }
}

void fcmp(ftregister_type freg, ftnumber_type num) {
  fcmp(xmm[freg], num);
}

void fcmp(ftregister_type freg1, ftregister_type freg2) {
  fcmp(xmm[freg1], xmm[freg2]);
}

//================================= ADD ========================================
void add(register_type reg, number_type num) {
  r[reg] += num;
}

void add(register_type reg1, register_type reg2) {
  add(reg1, r[reg2]);
}

void fadd(ftregister_type freg, ftnumber_type num) {
  xmm[freg] += num;
}

void fadd(ftregister_type freg1, ftregister_type freg2) {
  fadd(freg1, xmm[freg2]);
}

//================================= SUB ========================================
void sub(register_type reg, number_type num) {
  r[reg] -= num;
}

void sub(register_type reg1, register_type reg2) {
  sub(reg1, r[reg2]);
}

void fsub(ftregister_type freg, ftnumber_type num) {
  xmm[freg] -= num;
}

void fsub(ftregister_type freg1, ftregister_type freg2) {
  fsub(freg1, xmm[freg2]);
}

//================================= MUL ========================================
void mul(register_type reg, number_type num) {
  r[reg] *= num;
}

void mul(register_type reg1, register_type reg2) {
  mul(reg1, r[reg2]);
}

void fmul(ftregister_type freg, ftnumber_type num) {
  xmm[freg] *= num;
}

void fmul(ftregister_type freg1, ftregister_type freg2) {
  fmul(freg1, xmm[freg2]);
}

//================================= DIV ========================================
void fdiv(ftregister_type freg, ftnumber_type num) {
  xmm[freg] /= num;
}

void fdiv(ftregister_type freg1, ftregister_type freg2) {
  fdiv(freg1, xmm[freg2]);
}

//================================= SQRT =======================================
void fsqrt(ftregister_type freg) {
  xmm[freg] = sqrt(xmm[freg]);
}

//================================= LNOT =======================================
void lnot(register_type reg) {
  r[reg] = ~r[reg];
}

void flnot(ftregister_type freg) {
  uint64_t tmp = ~reinterpret_cast<uint64_t&>(xmm[freg]);
  xmm[freg] = reinterpret_cast<double&>(tmp);
}

//================================= INF ========================================
void finf(ftregister_type freg) {
  xmm[freg] = std::numeric_limits<double>::infinity();
}

//================================= JMP ========================================
void jmp(label_type pos) {
  pc = text + pos + sizeof(Command);
}

void jz(label_type pos) {
  if (ZF == 1) {
    jmp(pos);
  }
}

void jnz(label_type pos) {
  if (ZF == 0) {
    jmp(pos);
  }
}

void jl(label_type pos) {
  if (SF == 1) {
    jmp(pos);
  }
}

void jg(label_type pos) {
  if (SF == 0) {
    jmp(pos);
  }
}

void jle(label_type pos) {
  if (ZF == 1 || SF == 1) {
    jmp(pos);
  }
}

void jge(label_type pos) {
  if (ZF == 1 || SF == 0) {
    jmp(pos);
  }
}

//================================= CALL =======================================
void call(label_type pos) {
  push((number_type)pc);
  jmp(pos);
}

//================================= RET ========================================
void ret() {
  pop(13);
  pc = (char*)r[13];
}

//================================= END ========================================
void end() {
  pc = ram;
}

