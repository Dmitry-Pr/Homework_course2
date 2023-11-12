.include "macro-string.asm"
.include "macrolib.asm"

.globl main
.eqv     BUF_SIZE 10
.data
buf1:    .space BUF_SIZE     
buf2:    .space BUF_SIZE     
empty_test_str: .asciz ""   
short_test_str: .asciz "Hello!"     

.text
main:	print_str("Enter string to copy (8 symbols).\n")
	input_str_a0(buf1, BUF_SIZE)
	print_str_label(buf1)
    
	la      a0 buf2
	la      a1 buf1
	jal     strcpy
	
	print_str("Your copyied string\n")
	print_str_label(buf2)
	newline

    	# Копирование пустой строки
    	la      a0 buf2
    	la      a1 empty_test_str
    	jal     strcpy
    	print_str("Copying an empty string.\n")
    	print_str_label(buf2)
    	newline

    	# Копирование строки
	strcpy(buf2, short_test_str)
 	print_str("Copying a string from label.\n")
	print_str_label(buf2)
	newline

	exit

