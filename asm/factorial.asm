    in

    pop     r0      # r0 - result
    mov     r1 r0
.loop_begin:
    cmp     r1 1
    jle     .loop_end
    call    factorial
    jmp     .loop_begin
.loop_end:
    push    r0
    out
    end

factorial:
    sub     r1 1
    mul     r0 r1

    ret

