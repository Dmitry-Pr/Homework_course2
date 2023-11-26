.include "macrolib.asm"
.eqv    NAME_SIZE 256
.eqv    TEXT_SIZE 512
.global main, file_name, strbuf, correct, incorrect
.data
correct:
	.asciz "Correct!\n"
incorrect:
	.asciz "Incorrect!\n"
inp:	.asciz "Enter input filename."
outp:	.asciz "Enter output filename."
howout:	.asciz "Enter Y in order to write to console.\nEnter N or anythig else in order to write to file."
tests_or_prg:
	.asciz "Do you want to run tests for programm?\nEnter 1 in order to run tests. \nEnter 2 in order to run other tests.\nEnter anything else to run programm itself."
one:	.asciz "1\n"
two:	.asciz "2\n"
y:	.asciz "Y\n"
file_name:	
	.space	NAME_SIZE
strbuf:	.space TEXT_SIZE
input:	.space	100
inputtests:	
	.space	100
.text
main:	la	t0 tests_or_prg	# choosing between programm and tests
	input_dialog_str(t0, inputtests, 100)
	bnez	a1 prg
	strcmp(one, inputtests)
	beqz	a0 test1
	strcmp(two, inputtests)
	beqz	a0 test2
	
prg:	li	a0 0	# programm
	la	a1 inp
	jal input_filename
	bnez 	a1 again
	jal	read_file
	bnez 	a1 again
	jal 	check_brackets
	beqz	a0 false
	la	a1 correct
	push(a1)
	j 	out
false:	la	a1 incorrect
	push(a1)
	
out:	la	t0 howout
	input_dialog_str(t0, input, 100)
	bnez	a1 tofile
	strcmp(y, input)
	bnez	a0 tofile
	pop(a1)
	show_message(a1, 1)	# print
	j again
tofile:	li	a0 1	# WRITE
	la	a1 outp
	jal	input_filename
	pop(a1)
	jal write_file
	j again

test1:	jal	test1_programm
	j again
test2:	jal 	test2_programm
again:	repeat
	exit
