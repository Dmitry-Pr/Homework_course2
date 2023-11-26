.include "macrolib.asm"
.global write_file

.text
write_file:	
	# a0 - descriptor
	# a1 - buffer		
	push(ra)
	push(a0)
	mv	a0 a1
	jal strlen	# finding len of out string
	mv	a2 a0
	pop(a0)
    	li   a7 64	# writing
    	ecall
    	close(a0)	# closing
fin:	pop(ra)
	ret
