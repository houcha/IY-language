    # ax*x + bx + c = 0
    # xmm0 = a, xmm1 = b, xmm2 = c
    fin      xmm0
    fin      xmm1
    fin      xmm2
    # xmm3 = x1, xmm4 = x2
    fmov    xmm3 0
    fmov    xmm4 0

    fcmp    xmm0 0              # if a == 0 solve linear
    jz      solve_linear        #   else find discriminant
    # Find discriminant.
    # xmm3 = D
    fmov    xmm3 xmm1
    fmul    xmm3 xmm1           # xmm3 = b*b
    fmov    xmm4 4
    fmul    xmm4 xmm0
    fmul    xmm4 xmm2           # xmm4 = 4ac
    fsub    xmm3 xmm4           # xmm3 = b*b - 4ac
    fsqrt   xmm3                # xmm3 = sqrt(b*b - 4ac) = D
    fcmp    xmm3 0              # if D < 0 no roots
    jl      .print_no_roots
    fmov    xmm4 -1
    fmul    xmm4 xmm1
    fdiv    xmm4 2
    fdiv    xmm4 xmm0           # xmm4 = -b/2a
    fcmp    xmm3 0              # if D == 0 one root
    jl      .print_one_root
    fdiv    xmm3 2
    fdiv    xmm3 xmm0           # xmm3 = D/2a
    fmov    xmm1 xmm4           # xmm1 = -b/2a
    fmov    xmm2 xmm3           # xmm2 = D/2a

    fmov    xmm3 xmm1           # xmm3 = -b/2a
    fmov    xmm4 xmm1           # xmm4 = -b/2a

    fsub    xmm3 xmm2           # xmm3 = x1
    fadd    xmm4 xmm2           # xmm4 = x2
    jmp     .print_two_roots

.print_one_root:
    fout    xmm3
    jmp     .end
.print_two_roots:
    fout    xmm3
    fout    xmm4
    jmp     .end
.print_inf_roots:
    finf    xmm3
    fout    xmm3
    jmp     .end
.print_no_roots:
    fmov    xmm3 -1
    fsqrt   xmm3
    fout    xmm3

.end:
    end

solve_linear:
    # bx + c = 0
    fcmp    xmm1 0              # if b == 0 check equality
    jz      solve_equality      #   else x1 = -c/b
    fmul    xmm2 -1
    fdiv    xmm2 xmm1
    fmov    xmm3 xmm2
    jmp     .print_one_root

solve_equality:
    # c = 0
    fcmp    xmm2 0              # if c == 0 inf_roots
    jz      .print_inf_roots    #   else no_roots
    jmp     .print_no_roots

