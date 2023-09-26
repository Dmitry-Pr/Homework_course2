.data
max:	.word	0x7fffffff
min:	.word	-2147483648
sep:    .asciz  "Array is filled in.\n"
mes1:	.asciz	"Enter how many elements are there in array (0 < n < 11).\n"
mes2:	.asciz 	"Enter array members.\n"
mes3:	.asciz	"Incorrect number of elements in array.\n"
mes4:	.asciz	"Elements in sum.\n"
mes5:	.asciz	"\nSum.\n"
mes6:	.asciz	"\nNumber of even.\n"
mes7:	.asciz	"\nNumber of odd.\n"
.align  2                       
array:  .space  40              
arrend:                         
.text
        la      t0 array                
        la      t1 arrend         
        li	t2 10
        li	t3 0
        la 	a0 mes1
	li	a7 4
	ecall
        li 	a7 5
	ecall
	blez	a0 zer
	bgt	a0 t2 zer
	mv	t2 a0
	la 	a0 mes2
	li	a7 4
	ecall
	li 	a7 5
fill:   beq	t2 t3 filled
	ecall
	addi	t3 t3 1
	sw     	a0 (t0)                  
        addi    t0 t0 4         
        bltu    t0 t1 fill
filled:	la      a0 sep          
        li      a7 4
        ecall	
        mv 	t1 t2	# len of array
        li 	t3 0	# count
        li 	t2 0	# sum
        la 	t0 array # array
        lw	t4 max	# max
        lw	t5 min	# min
loops:	lw	a0 (t0)	# current
	bltz  	a0 ltz
	sub 	t6 t4 a0 # max - current
	bgt	t2 t6 prints
	j 	corr
ltz:	sub	t6 t5 a0 # min - current
	blt	t2 t6 prints
corr:	add	t2 t2 a0
	addi 	t3 t3 1
	addi 	t0 t0 4
	bltu    t3 t1 loops
prints:	la 	a0 mes4
	li	a7 4
	ecall
	li	a7 1
	mv	a0 t3
	ecall
	la 	a0 mes5
	li	a7 4
	ecall
	li	a7 1
	mv	a0 t2
	ecall
	li 	t3 0	# count
	li 	t4 0	# odd
	li	t5 0	# even
	la      t0 array
loopoe:	lw	a0 (t0)
	andi	t6 a0 1
	beqz	t6 even
	addi 	t4 t4 1
	addi	t0 t0 4
	addi	t3 t3 1
	bltu    t3 t1 loopoe
	j printoe
even:	addi	t5 t5 1
addi	t0 t0 4
	addi	t3 t3 1
	bltu    t3 t1 loopoe
printoe:la 	a0 mes6
	li	a7 4
	ecall
	li	a7 1
	mv	a0 t5
	ecall
	la 	a0 mes7
	li	a7 4
	ecall
	li	a7 1
	mv	a0 t4
	ecall
        j	end
zer:	la 	a0 mes3
	li	a7 4
	ecall
end:	li      a7 10           
        ecall
