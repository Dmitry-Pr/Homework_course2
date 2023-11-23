.include "macrolib.asm"
.global clear_digit
.text
clear_digit:
	li	t0 0x80
	sb	t0 (a1)
	ret
