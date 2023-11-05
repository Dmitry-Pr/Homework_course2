.macro print_int (%x)
	li a7 1
	mv a0 %x
	ecall
.end_macro

.macro print_imm_int (%x)
	li a7 1
    	li a0 %x
    	ecall
.end_macro

.macro read_int_a0
   	li a7 5
   	ecall
.end_macro

.macro read_int(%x)
   	push	(a0)
   	li a7 5
   	ecall
   	mv %x, a0
   	pop	(a0)
.end_macro

.macro read_double_fa0
   	li a7 7
   	ecall
.end_macro

.macro read_double(%x)
   	push_double(fa0)
   	li a7 7
   	ecall
   	fmv %x fa0
   	pop_double(fa0)
.end_macro

.macro print_double (%x)
	push_double(fa0)
	li a7 3
	fmv.d fa0 %x
	ecall
	pop_double(fa0)
.end_macro

.macro double_li (%d, %imm)
	push(a0)
	li	a0 %imm
	fcvt.d.w %d a0
	pop(a0)
.end_macro

.macro print_str (%x)
.data
str:	.asciz %x
.text
   	push (a0)
   	li a7 4
   	la a0 str
   	ecall
   	pop	(a0)
.end_macro

.macro print_char(%x)
   	li a7 11
   	li a0 %x
   	ecall
.end_macro

.macro newline
   	print_char('\n')
.end_macro

.macro exit
    	li a7 10
    	ecall
.end_macro

.macro push(%x)
	addi	sp sp -4
	sw	%x (sp)
.end_macro

.macro pop(%x)
	lw	%x (sp)
	addi	sp sp 4
.end_macro

.macro push_double(%x)
	addi	sp sp -8
	fsd	%x (sp)
.end_macro

.macro pop_double(%x)
	fld	%x (sp)
	addi	sp sp 8
.end_macro

.macro repeat
	print_str("Do you want to continue? Enter 1 in order to repeat programm or another integer in order to stop.\n")
	read_int_a0
	li	t0 1
	beq	a0 t0 main
.end_macro

.macro read_accuracy
.data
left:	.double	0.00000001
right:	.double	0.001
.text
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
fin:
.end_macro

.macro print_results (%x, %acc)
	print_str("On the interval [0;1] with accuracy of calculations ")
	print_double(%acc)
	print_str(" found the root: ")
	print_double(%x)
	newline
.end_macro