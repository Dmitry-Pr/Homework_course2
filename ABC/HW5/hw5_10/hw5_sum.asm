.include "macrolib.s"
.global sum

.data
max:	.word	0x7fffffff
min:	.word	-2147483648
.text
sum:
	mv	t1 a0	# len of array
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
prints:	mv	a0 t3
	mv	a1 t2
	ret