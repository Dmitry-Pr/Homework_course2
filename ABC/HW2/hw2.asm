.data
    arg01:  .asciz "Введите делимое: "
    arg02:  .asciz "Введите делитель: "
    ln:     .asciz "\n"
    zeroex: .asciz "На ноль делить нельзя! Программа завершена.\n"
    fnumber:.asciz "Частное: "
    snumber:.asciz "Остаток: "
    
.text
	la a0 arg01
        li a7 4
        ecall
        li a7 5
        ecall
        mv s0 a0
        la a0 arg02
        li a7 4
        ecall
        li a7 5
        ecall
        mv s1 a0
        bgtz s1 ifgtz
        li t6 2
        ifgtz:li s2 0
        bnez s1 prog
        la a0 zeroex
        li a7 4
        ecall
        li a7 10
        prog:blt s2 s1 gtz
        neg s1 s1
        neg s0 s0
        gtz:li s3 2
        blt s0 s2 ltz
        li s3 1
        blt s0 s1 ltz
        loop:sub s0 s0 s1
        addi s2 s2 1
        bge s0 s1 loop
        li t0 1
        ltz:beq s3 t0 fin
	neg t0 s0
	blt t0 s1 fin
	loop2:add s0 s0 s1
        addi s2 s2 -1
        neg t0 s0
        bge t0 s1 loop2
        fin:la a0 fnumber
        li a7 4
        ecall
        mv a0 s2
        li a7 1
        ecall
        la a0 ln
        li a7 4
        ecall
        la a0 snumber
        li a7 4
        ecall
        li t5 2
        bne t5 t6 correct
        neg s0 s0
        correct:mv a0 s0
        li a7 1
        ecall
        
