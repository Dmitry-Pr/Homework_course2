.text
.globl strcpy
strcpy:	# a0 - destination, a1 - source
	mv	t2 a0
loop:	lb	t0 (a1)	
	sb	t0 (a0)	
	beqz	t0 end	
	addi	a0 a0 1	
	addi	a1 a1 1
	b	loop
end:	mv	a0 t2	# a0 - destination
	ret
