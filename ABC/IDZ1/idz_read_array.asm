.include "macrolib.s"
.global read_array

.text
read_array:
	push(ra)
	mv      t0 a0	# array            
        mv      t1 a1	# arrend         
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
	mv	a0 t2	# len of array
	li 	a1 0	# filled in correctly
	ret
zer:	print_str("Incorrect number of elements in array. Programm is stopped.\n")
	li	a0 0
	li	a1 1	# filled in incorrectly
	pop(ra)
	ret	
