.include "idz2_macrolib.asm"
.global find_root

.text
find_root:	# fa0 contains accuracy of calculations
	fmv.d	fa1 fa0		# fa1 = eps
	push(ra)
	double_li(ft0, 0)	# ft0 = a0
	double_li(ft1, 1)	# ft1 = b0
	double_li(ft2, 2)
loop:	fadd.d	ft3 ft0 ft1
	fdiv.d	ft3 ft3 ft2	# ft3 = ck = (ak + bk) / 2
	
	fmv.d 	fa0 ft0
	# parameter fa0 - x
	# out parameter fa0 - f(x)
	jal function
	fmv.d 	ft4 fa0		# ft4 = f(ak)
	
	fmv.d 	fa0 ft3
	# parameter fa0 - x
	# out parameter fa0 - f(x)
	jal function
	fmv.d 	ft5 fa0		# ft5 = f(ck)
	
	fmul.d	ft4 ft4 ft5	# ft4 = f(ak) * f(ck)
	double_li(ft6, 0)
	flt.d	t0 ft4 ft6
	beqz	t0 move_right
	fmv.d	ft1 ft3		# ft1 = bk+1 = ck
	j check_loop
	
move_right:
	fmv.d	ft0 ft3		# ft0 = ak+1 = ck
	
check_loop:
	fsub.d	ft6 ft1 ft0
	fgt.d	t0 ft6 fa1	# b - a > eps
	bnez	t0 loop
	
	fadd.d	ft3 ft0 ft1
	fdiv.d	ft3 ft3 ft2
	fmv.d	fa0 ft3		# fa0 = (a + b) / 2
fin:	pop(ra)
	ret
