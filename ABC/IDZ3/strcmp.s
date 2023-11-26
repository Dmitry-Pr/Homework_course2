.text
.globl strcmp
strcmp:
loop:
    lb      t0 (a0)     
    lb      t1 (a1)     
    beqz    t0 end      
    beqz    t1 end      
    bne     t0 t1 end   
    addi    a0 a0 1     
    addi    a1 a1 1     
    b       loop
end:
    sub     a0 t0 t1    
    ret
