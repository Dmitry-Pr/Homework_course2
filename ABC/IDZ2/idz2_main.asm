.include "idz2_macrolib.asm"

.global main

.text
main:	print_str("Do you want to run tests for programm?\nEnter 1 in order to run tests.\nEnter any other integer to run programm itself.\n")
	read_int_a0
	li	t0 1
	bne	a0 t0 prg
	jal test
	j again	
	
	# out parameter fa0 - accuracy
	# out parameter a1 - flag that shows if input is correct
	# you can comment next line and uncomment line with macro 
prg:	jal input
	# read_accuracy
	bnez 	a1 again
	push_double(fa0)	# saving accuracy on stack
	
	# parameter fa0 - accuracy
	# out parameter fa0 - x
	jal find_root
	pop_double(fa1)		# getting accuracy from stack
	# parameter fa0 - x
	# parameter fa1 - accuracy
	jal output
	
again:	repeat
	exit
