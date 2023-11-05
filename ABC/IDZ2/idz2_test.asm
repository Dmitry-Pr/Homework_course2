.include "idz2_macrolib.asm"

.global test
.text
test:

.data
eps1:	.double 0.001
eps2:	.double 0.0001
eps3:	.double 0.00000001
.text
	push(ra)
	
	print_str("Test 1\n")
	fld	fa0 eps1 t0
	print_str("Current:\n")
	# parameter fa0 - accuracy
	# out parameter fa0 - x
	jal find_root
	fld	fa1 eps1 t0
	# parameter fa0 - x
	# parameter fa1 - accuracy
	jal output
	print_str("Correct:\n")
	print_str("On the interval [0;1] with accuracy of calculations ")
	print_double(fa1)
	print_str(" found the root: 0.86669921875\n")
	
	print_str("Test 2\n")
	fld	fa0 eps2 t0
	print_str("Current:\n")
	# parameter fa0 - accuracy
	# out parameter fa0 - x
	jal find_root
	fld	fa1 eps2 t0
	# parameter fa0 - x
	# parameter fa1 - accuracy
	print_results(fa0, fa1)
	print_str("Correct:\n")
	print_str("On the interval [0;1] with accuracy of calculations ")
	print_double(fa1)
	print_str(" found the root: 0.866790771484375\n")
	
	print_str("Test 3\n")
	fld	fa0 eps3 t0
	print_str("Current:\n")
	# parameter fa0 - accuracy
	# out parameter fa0 - x
	jal find_root
	fld	fa1 eps3 t0
	# parameter fa0 - x
	# parameter fa1 - accuracy
	jal output
	print_str("Correct:\n")
	print_str("On the interval [0;1] with accuracy of calculations ")
	print_double(fa1)
	print_str(" found the root: 0.8667603991925716\n")
	
endt:	pop(ra)
	ret
