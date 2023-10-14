.macro print_int (%x)
	li a7, 1
	mv a0, %x
	ecall
.end_macro

.macro print_imm_int (%x)
	li a7, 1
    	li a0, %x
    	ecall
.end_macro

.macro read_int_a0
   	li a7, 5
   	ecall
.end_macro

.macro read_int(%x)
   	push	(a0)
   	li a7, 5
   	ecall
   	mv %x, a0
   	pop	(a0)
.end_macro

.macro print_str (%x)
.data
str:	.asciz %x
.text
   	push (a0)
   	li a7, 4
   	la a0, str
   	ecall
   	pop	(a0)
.end_macro

.macro print_char(%x)
   	li a7, 11
   	li a0, %x
   	ecall
.end_macro

.macro newline
   	print_char('\n')
.end_macro

.macro exit
    	li a7, 10
    	ecall
.end_macro

.macro push(%x)
	addi	sp, sp, -4
	sw	%x, (sp)
.end_macro

.macro pop(%x)
	lw	%x, (sp)
	addi	sp, sp, 4
.end_macro

.macro repeat
	print_str("Do you want to continue? Enter 1 in order to repeat programm or another integer in order to stop.\n")
	read_int_a0
	li	t0 1
	beq	a0 t0 main
.end_macro

.macro fill_array_from_1(%arr, %arrend)
	li	t0 1 # counter
	la	t1 %arr
	la	t2 %arrend
loop:	sw	t0 (t1)
	addi 	t1 t1 4
	addi 	t0 t0 1
	blt	t1 t2 loop
.end_macro

.macro	print_array(%arr, %len)
	print_str("Array members:\n")
	la      t0 %arr	# array
	mv	t1 %len	# len of array
	li 	t3 0	# count
loopprt:lw	t2 (t0)
	addi 	t0 t0 4
	print_int(t2)
	newline
	addi	t3 t3 1
	blt	t3 t1 loopprt
.end_macro 

.macro read_array(%array, %arrend)
	la      t0 %array	# array            
        la      t1 %arrend	# arrend         
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
	j 	return
zer:	print_str("Incorrect number of elements in array. Programm is stopped.\n")
	li	a0 0
	li	a1 1	#filled in incorrectly
return:
.end_macro
