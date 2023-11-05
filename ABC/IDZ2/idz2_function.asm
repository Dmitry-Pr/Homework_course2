.include "idz2_macrolib.asm"
.global function

.text
function: # fa0 contains x
	push_double(ft0) # saving locals of outside
	push_double(ft1)
	push_double(ft2)
	push_double(ft3)
	push_double(ft4)
	push(ra)
	fmul.d	ft0 fa0 fa0	# ft0 = x * x
	li	t0 2
	fcvt.d.w ft1 t0		# ft1 = 2
	fmul.d	ft2 fa0 ft0	# ft2 = x * x * x
	fmsub.d	ft3 ft1 ft2 fa0	# ft3 = 2x^3 - x
	li	t0 1
	fcvt.d.w ft1 t0		# ft1 = 1
	fmsub.d ft4 ft0 ft0 ft1 # ft4 = x^4 - 1
	fadd.d 	ft4 ft4 ft3	# ft4 = x^4 + 2x^3 - x - 1
	fmv.d	fa0 ft4
fin:	pop(ra)
	pop_double(ft4)
	pop_double(ft3)
	pop_double(ft2)
	pop_double(ft1)
	pop_double(ft0)
	ret
