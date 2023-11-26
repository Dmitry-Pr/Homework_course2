.include "macrolib.asm"
.global check_brackets

.text
check_brackets:	# a0 - text to check 
	push(ra)
	push(t0)
	push(t1)
	push(t2)
	push(t3)
	li	t0 0
	li	t1 0x28	# (
	li	t2 0x29	# )
loop:	lb	t3 (a0)	# reading a symbol
	beqz	t3 fin
	addi	a0 a0 1
	beq	t3 t1 left
	beq	t3 t2 right
	j	loop
left:	addi	t0 t0 1	# if ( adding 1 to count
	j loop
right:	addi	t0 t0 -1	# if ) adding -1 to count
	bltz	t0 false	# if count < 0 => incorrect sequence
	j	loop
fin:	bnez	t0 false	# if count != 0 => incorrect sequence
	li	a0 1	# correct	
	j	return
false:	li	a0 0	# incorrect
return:	pop(t3)
	pop(t2)
	pop(t1)
	pop(t0)
	pop(ra)
	ret
