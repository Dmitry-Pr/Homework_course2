.text
	jal maxfact
	li	a7 1
	ecall
	li	a7 10
	ecall
	
maxfact:li	t0 1 # fact
	li	t1 0 # counter
loop:	addi 	t1 t1 1
	mulhu	t2 t0 t1
	mul	t0 t0 t1
	bgtz	t2 return
	j loop
return:	addi	t1 t1 -1
	mv	a0 t1
	ret