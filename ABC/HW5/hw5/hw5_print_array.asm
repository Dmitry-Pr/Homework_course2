.include "macrolib.s"
.global print_array

.text
print_array:
	print_str("Array members:\n")
	la 	t0 array # array
	mv	t1 a0	# len of array
	li 	t3 0	# count
loopprt:lw	t2 (t0)
	addi 	t0 t0 4
	print_int(t2)
	newline
	addi	t3 t3 1
	blt	t3 t1 loopprt
	ret
