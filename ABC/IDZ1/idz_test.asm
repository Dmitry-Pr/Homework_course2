.include "macrolib.s"

.global test
.text
test:

.data
arr1:	 .space 40
arrend1:
arr1B:	 .space 40
arrend1B:
arr2: 	.space 40
arrend2:
arr2B: 	.space 40
arrend2B:
arr3:	.space 40
arrend3:
arr3B:	.space 40
arrend3B:
.text
	# {1} 
	# {1 2 3 4}
	# {1 2 3 4 5}
	# for full tests coverage it is enough to test these arrays
	# an array of len 1
	# an array of even len
	# an arary of odd len
	push(ra)
test1:	fill_array_from_1(arr1, arrend1)
	li	a0 1 # len of array 
	print_str("Test array 1\n")
	print_array(arr1, a0)
	la	a0 arr1
	la	a1 arr1B
	li	t0 1
	push(a0)
	push(a1)
	push(t0)
	# parameters are saved on stack
	# parameter a0 - link to arrayA in memory
	# parameter a1 - link to arrayB in memory
	# parameter t0 - len of arrayA
	jal group
	print_str("Result array 1\n")
	li	a0 1 # len of array 
	print_array(arr1B, a0)
	
	#check blok 1
	la	t0 arr1B
	lw	t1 (t0)	# current element
	li	t2 1	# correct element
	beq	t2 t1 correct1
	print_str("Failed test 1.\n")
	j test2
correct1:
	print_str("Passed test 1.\n")
	
test2:	fill_array_from_1(arr2, arrend2)
	li	a0 4 # len of array 
	print_str("Test array 2.\n")
	print_array(arr2, a0)
	
	la	a0 arr2
	la	a1 arr2B
	li	t0 4
	push(a0)
	push(a1)
	push(t0)
	# parameters are saved on stack
	# parameter a0 - link to arrayA in memory
	# parameter a1 - link to arrayB in memory
	# parameter t0 - len of arrayA
	jal group
	print_str("Result array 2.\n")
	li	a0 4 # len of array 
	print_array(arr2B, a0)
	
	#check blok 2
	la	t0 arr2B
	lw	t1 (t0) # current element 	
	li	t3 1	# correct element
	bne	t1 t3 failed2
	li	t3 3
	addi	t0 t0 4
	lw 	t1 (t0)
	bne	t1 t3 failed2
	li	t3 2
	addi	t0 t0 4
	lw 	t1 (t0)
	bne	t1 t3 failed2
	li	t3 4
	addi	t0 t0 4
	lw 	t1 (t0)
	bne	t1 t3 failed2
	print_str("Passed test 2.\n")
	j	test3
failed2: print_str("Failed test 2.\n")
	
test3:	fill_array_from_1(arr3, arrend3)
	li	a0 5 # len of array 
	print_str("Test array 3.\n")
	print_array(arr3, a0)
	
	la	a0 arr3
	la	a1 arr3B
	li	t0 5
	push(a0)
	push(a1)
	push(t0)
	# parameters are saved on stack
	# parameter a0 - link to arrayA in memory
	# parameter a1 - link to arrayB in memory
	# parameter t0 - len of arrayA
	jal group
	print_str("Result array 3.\n")
	li	a0 5 # len of array 
	print_array(arr3B, a0)
	
	#check blok 3
	la	t0 arr3B
	lw	t1 (t0) # current element 	
	li	t3 1	# correct element
	bne	t1 t3 failed3
	li	t3 3
	addi	t0 t0 4
	lw 	t1 (t0)
	bne	t1 t3 failed3
	li	t3 5
	addi	t0 t0 4
	lw 	t1 (t0)
	bne	t1 t3 failed3
	li	t3 2
	addi	t0 t0 4
	lw 	t1 (t0)
	bne	t1 t3 failed3
	li	t3 4
	addi	t0 t0 4
	lw 	t1 (t0)
	bne	t1 t3 failed3
	print_str("Passed test 3.\n")
	j	endt
failed3: print_str("Failed test 3.\n")
	
endt:	pop(ra)
	ret
