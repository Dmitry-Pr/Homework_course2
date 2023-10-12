.include "macrolib.s"
.global main
.global array
.global arrend

.data
array:  .space  40              
arrend:

.text
main:	
	jal	read_array
	push(a0) # array len
	jal	sum
	print_str("Elements in sum.\n")
	print_int(a0)
	print_str("\nSum.\n")
	print_int(a1)
	newline
	pop(a0)
	jal	print_array
	exit
