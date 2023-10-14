.include "macrolib.s"
.global group

.text
group:
	pop(t0)	# len of array
	pop(t1)	# arrayB
	pop(t2)	# arrayA
	push(ra)
	push(t2)# saving local value of arrayA
	li	t3 0	# counter
mkarr:	lw	t4 (t2)	# current in A
	sw	t4 (t1)	# save element in B
	addi	t3 t3 2
	addi	t2 t2 8	# add 2 to A index
	addi	t1 t1 4	# move B to next element
	blt	t3 t0 mkarr
	andi	t5 t3 1	# checking if added odd indexes or even
	bne	t5 zero fin # if added odd
	li	t3 1
	pop(t2)	# saved arrayA
	beq	t3 t0 fin # if len of A is 1
	addi	t2 t2 4 # first element to add to B is A[1]
	j	mkarr
fin:	pop(ra)
	ret
