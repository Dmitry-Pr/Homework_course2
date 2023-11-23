.include "macrolib.asm"
.global show_digit
.data
digits:
	.word 0x3f
	.word 0x6
	.word 0x5b
	.word 0x4f
	.word 0x66
	.word 0x6d
	.word 0x7d
	.word 0x7
	.word 0x7f
	.word 0x6f
	.word 0x77
	.word 0x7c
	.word 0x39
	.word 0x5e
	.word 0x79
	.word 0x71
end_digits:
.text
show_digit:
	push(ra)
	la	t0 digits
	li	t1 15
	bgt	a0 t1 overf
	li	t1 4
	mul	a0 a0 t1
	add	t0 t0 a0
	lw	t2 (t0)
	sb	t2 (a1)
	j end
overf:	li	t1 16
	div	t2 a0 t1
	mul	t1 t1 t2
	sub	a0 a0 t1
	li	t1 4
	mul	a0 a0 t1
	add	t0 t0 a0
	lw	t2 (t0)
	addi	t2 t2 0x80
	sb	t2 (a1)
end:	pop(ra)
	ret
	