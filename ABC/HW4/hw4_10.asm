.text
	li 	a2 100
	jal	fact2
	li	a7 1
	ecall
	li	a7 10
	ecall

fact2:	addi	sp sp -8
	sw	ra 4(sp)
	sw	s1 (sp)
	
	li 	t0 1
	mv	s1 a2
	addi	a2 a2 -1
	ble	a2 t0 done
	jal	fact2
	beq	a1 t0 skip
	mulhu	t2 s1 a2
	bgtz	t2 return2
	mul	s1 s1 a2
done:	mv	a2 s1
	j skip
return2:li	a1 1
	addi	a0 s1 -1
skip:	lw	s1 (sp)
	lw	ra 4(sp)
	addi	sp sp 8
	ret
