.macro	read_array(%array, %arrend)
	la      t0 %array                
        la      t1 %arrend         
        li	t2 10
        li	t3 0
	print_str("Enter how many elements are there in array (0 < n < 11).\n")
	read_int_a0
	blez	a0 zer
	bgt	a0 t2 zer
	mv	t2 a0
	print_str("Enter array members.\n")
fill:   beq	t2 t3 filled
	read_int_a0
	addi	t3 t3 1
	sw     	a0 (t0)                  
        addi    t0 t0 4         
        bltu    t0 t1 fill
filled:	print_str("Array is filled in.\n")
	mv	a0 t2
	j 	end
zer:	print_str("Incorrect number of elements in array.\n")
	exit	
end:
.end_macro

.macro	sum_array(%array, %len)
.data
max:	.word	0x7fffffff
min:	.word	-2147483648
.text
	mv	t1 %len	# len of array
        li 	t3 0	# count
        li 	t2 0	# sum
        la 	t0 %array # array
        lw	t4 max	# max
        lw	t5 min	# min
loops:	lw	a0 (t0)	# current
	bltz  	a0 ltz1
	sub 	t6 t4 a0 # max - current
	bgt	t2 t6 prints
	j 	corr
ltz1:	sub	t6 t5 a0 # min - current
	blt	t2 t6 prints
corr:	add	t2 t2 a0
	addi 	t3 t3 1
	addi 	t0 t0 4
	bltu    t3 t1 loops
prints:	mv	a0 t3
	mv	a1 t2
.end_macro

.macro	print_array(%array, %len)
	print_str("Array members:\n")
	la 	t0 %array # array
	mv	t1 %len	# len of array
	li 	t3 0	# count
loopprt:lw	t2 (t0)
	addi 	t0 t0 4
	print_int(t2)
	newline
	addi	t3 t3 1
	blt	t3 t1 loopprt
.end_macro