.include "macrolib.asm"

.global test1_programm
.data
inpt1:	.asciz "input\\test1.txt"
inpt2:	.asciz "input\\test2.txt"
inpt3:	.asciz "input\\test3.txt"
inpt4:	.asciz "input\\test4.txt"
outt1:	.asciz "input\\out1.txt"
outt2:	.asciz "input\\out2.txt"
outt3:	.asciz "input\\out3.txt"
outt4:	.asciz "input\\out4.txt"
inper:
	.asciz "Problems with files. Programm is stopped.\n"
.text
test1_programm:
	push(ra)
tstl:	li	s0 0	# test1
	open(inpt1, s0)
	li	s1 -1
    	beq	a0 s1 ert	# a0 - result descriptor
    	j 	read_test1
read_test1:
	jal	read_file
	jal 	check_brackets
	beqz	a0 falt1
	la	a1 correct
	push(a1)
	j 	out1
falt1:	la	a1 incorrect
	push(a1)
	
out1:	li	s0 1
	open(outt1, s0)
	li	s1 -1
    	beq	a0 s1 ert
	pop(a1)
	jal write_file
	


tst2:	li	s0 0	# test2
	open(inpt2, s0)
	li	s1 -1
    	beq	a0 s1 ert	# a0 - result descriptor
    	j 	read_test2

read_test2:
	jal	read_file
	jal 	check_brackets
	beqz	a0 falt2
	la	a1 correct
	push(a1)
	j 	out2
falt2:	la	a1 incorrect
	push(a1)
	
out2:	li	s0 1
	open(outt2, s0)
	li	s1 -1
    	beq	a0 s1 ert
	pop(a1)
	jal write_file
	
tst3:	li	s0 0	# test3
	open(inpt3, s0)
	li	s1 -1
    	beq	a0 s1 ert	# a0 - result descriptor
    	j 	read_test3

read_test3:
	jal	read_file
	jal 	check_brackets
	beqz	a0 falt3
	la	a1 correct
	push(a1)
	j 	out3
falt3:	la	a1 incorrect
	push(a1)
	
out3:	li	s0 1
	open(outt3, s0)
	li	s1 -1
    	beq	a0 s1 ert
	pop(a1)
	jal write_file	
	
	
tst4:	li	s0 0	# test4
	open(inpt4, s0)
	li	s1 -1
    	beq	a0 s1 ert	# a0 - result descriptor
    	j 	read_test4

read_test4:
	jal	read_file
	jal 	check_brackets
	beqz	a0 falt4
	la	a1 correct
	push(a1)
	j 	out4
falt4:	la	a1 incorrect
	push(a1)
	
out4:	li	s0 1
	open(outt4, s0)
	li	s1 -1
    	beq	a0 s1 ert
	pop(a1)
	jal write_file
	
	
	
	j	endt1
ert:	
	la	a0 inper
	show_message(a0, 1)
	j	endt1
endt1:	pop(ra)
	ret
