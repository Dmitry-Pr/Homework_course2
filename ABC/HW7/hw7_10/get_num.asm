.include "macrolib.asm"
.global get_num
.text

get_num:
	push(ra)
	push(t0)
	push(t1)
	push(t2)
	li	t0 16
	div	t0 a0 t0
	mv	t2 t0
	log2(t0)
	li	t1 4
	mul	t2 t2 t1
	mul	t1 t1 t2
	sub	t1 a0 t1
	log2(t1)
	li	t2 4
	mul	t1 t1 t2
	add	a0 t0 t1
	pop(t2)
	pop(t1)
	pop(t0)
	pop(ra)
	ret

