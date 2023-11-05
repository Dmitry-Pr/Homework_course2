.include "idz2_macrolib.asm"
.global input

.data
left:	.double	0.00000001
right:	.double	0.001
.text
input:
	push(ra)
	print_str("Enter accuracy of calculations. It must be a number between 0.00000001 and 0.001.\n")
	read_double_fa0
	fld 	ft0 left t0
	fld	ft1 right t0
	flt.d 	t0 fa0 ft0
	bnez	t0 incorr
	fgt.d	t0 fa0 ft1
	bnez	t0 incorr
	li	a1 0
	j 	fin
incorr:	li 	a1 1
	print_str("Incorrect accuracy. Programm is stopped.\n")
fin:	pop(ra)
	ret