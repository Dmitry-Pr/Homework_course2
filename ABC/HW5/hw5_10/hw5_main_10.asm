.include "macrolib.s"
.include "hw5_10_macrolib.asm"
.global main
.global array
.global arrend

.data
array:  .space  40              
arrend:

.text
main:	
	read_array(array, arrend)
	push(a0) # array len
	sum_array(array, a0)
	print_str("Elements in sum.\n")
	print_int(a0)
	print_str("\nSum.\n")
	print_int(a1)
	newline
	pop(a0)
	print_array(array, a0)
	exit
