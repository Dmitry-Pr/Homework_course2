.text
.globl strlen
strlen:
    li      t0 0        
loop:
    lb      t1 (a0)   
    beqz    t1 end
    addi    t0 t0 1		
    addi    a0 a0 1		
    b       loop
end:
    mv      a0 t0
    ret
fatal:
    li      a0 -1
    ret
