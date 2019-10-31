in

loop:
    pop     r0
    sub     r0 1
    push    r0
    mul
    cmp     r0 1
    jnz     loop

out

