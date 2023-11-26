.include "macrolib.asm"
.eqv    NAME_SIZE 256
.eqv	READ_ONLY 0
.eqv	WRITE_ONLY 1
.global input_filename

.data
prompt:	.asciz "Enter filename."
fnamemsg:
	.asciz "Incorrect filename. Programm is stopped.\n"
.text
input_filename:	#a0 - mode
		#a1 - prompt
	push(ra)
	input_dialog_str(a1, file_name, NAME_SIZE)	# asking filename
	bltz	a1 er_name
	push(s0)	# removing \n 
	push(s1)
    	push(s2)
    	li	s0 '\n'
    	la	s1 file_name
next:
    	lb	s2  (s1)
    	beq 	s0 s2 replace
    	addi 	s1 s1 1
    	b	next
replace:
    	sb	zero (s1)
    	pop(s2)
    	pop(s1)
    	pop(s0)
	open(file_name, a0)	# opening file
    	li	s1 -1
    	beq	a0 s1 er_name	# a0 - result descriptor
    	j 	fin
er_name:	# in case of some errors
	li 	a1 1
	la	a0 fnamemsg
	show_message(a0, 1)
fin:	pop(ra)
	ret
