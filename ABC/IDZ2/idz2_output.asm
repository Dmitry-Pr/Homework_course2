.include "idz2_macrolib.asm"
.global output

.text
output:	# fa0 contains x, fa1 contains accuracy of calculations
	push(ra)
	print_str("On the interval [0;1] with accuracy of calculations ")
	print_double(fa1)
	print_str(" found the root: ")
	print_double(fa0)
	newline
fin:	pop(ra)
	ret