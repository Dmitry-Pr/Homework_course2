.include "macrolib.s"

.global main

.data
.align 2
arrayA:	.space	40 # max len of array is 10       
arrendA:
.align 2
arrayB:	.space	40
arrendB:

.text
main:	print_str("Do you want to run tests for programm?\nEnter 1 in order to run tests.\nEnter any other integer to run programm itself.\n")
	read_int_a0
	li	t0 1
	bne	a0 t0 prg
	jal test
	j again	
prg:	la	a0 arrayA
	la	a1 arrendA
	# parameter a0 - link to array in memory
	# parameter a1 - link to end of array in memory
	# out parameter a0 - len of array
	# out parameter a1 - flag that shows if input data is correct
	# you can comment next line and uncomment line with macro read_array, results will not change
	jal	read_array
	#read_array(arrayA, arrendA)
	li	t0 1
	beq	a1 t0 again # if input data is incorrect
	push(a0)	# saving len of array
	mv	t0 a0
	la	a0 arrayA
	la	a1 arrayB
	push(a0)
	push(a1)
	push(t0)
	
	# parameters are saved on stack
	# parameter a0 - link to arrayA in memory
	# parameter a1 - link to arrayB in memory
	# parameter t0 - len of arrayA
	jal	group
	
	la	a0 arrayB
	pop(a1)
	
	# parameter a0 - link to array in memory
	# parameter a1 - len of array
	jal	print_array
	
again:	repeat
	exit
