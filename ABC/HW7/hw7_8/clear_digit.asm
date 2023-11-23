.include "macrolib.asm"
.global clear_digit
.text
clear_digit:
	mv	t0 zero
	sb	t0 (a1)
	ret