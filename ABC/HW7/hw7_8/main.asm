.include "macrolib.asm"
.global main
.text
main:
	li	t5 0xffff0010  
	li	t6 0xffff0011
	print_str("Enter a positive number\n")
	read_int_a0
	li	t4 0
	li	t3 10
	mv	s1 a0
loop:	mv	a0 s1
	mv	a1 t5
	jal show_digit
	sleep(1000)
	jal clear_digit
	mv	a0 s1
	mv	a1 t6
	jal show_digit
	sleep(1000)
	jal clear_digit
	addi	t4 t4 1
	blt	t4 t3 loop
	exit