.macro	input_file(%mode, %label)
	push(a0)
	push(a1)
	li	a0 %mode
	la	a1 %label
	jal input_filename
	pop(a1)
	pop(a0)
.end_macro

.macro	read_file(%descriptor)
	mv	a0 %descriptor
	jal read_file
.end_macro

.macro	check_brackets(%text)
	mv 	a0 %text
	jal check_brackets
.end_macro

.macro write_file(%descriptor, %buf)
	mv	a0 %descriptor
	mv	a1 %buf
	jal write_file
.end_macro

.macro print_int (%x)
	push(a0)
	li a7 1
	mv a0 %x
	ecall
	pop(a0)
.end_macro

.macro	input_dialog_str(%prompt, %buf, %size)
.text
	push(a0)
	push(a2)
	mv	a0 %prompt
	la	a1 %buf
	li	a2 %size
	li	a7 54
	ecall
	pop(a2)
	pop(a0)
.end_macro

.macro print_imm_int (%x)
	push(a0)
	li a7 1
    	li a0 %x
    	ecall
    	pop(a0)
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
	push(a0)
   	li a7 11
   	li a0 %x
   	ecall
   	pop(a0)
.end_macro

.macro newline
   	print_char('\n')
.end_macro

.macro exit
    	li a7 10
    	ecall
.end_macro

.macro sleep(%time)
	push(a0)
	li a0 %time
    	li a7 32
    	ecall
    	pop(a0)
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
.data
repmsg:	.asciz "Do you want to continue? Enter 1 in order to stop or anything else in order to repeat programm."
ans:	.space 100
.text
	la	t0 repmsg
	input_dialog_str(t0, ans, 100)
	bnez	a1 main
	strcmp(one, ans)
	bnez	a0 main
.end_macro

.macro log2(%n)
	push(s0)
	push(s1)
	push(s2)
	mv	s0 %n
	li	s1 0
	li	s2 2
loop:	div	s0 s0 s2
	beqz	s0 fin
	addi	s1 s1 1
	j loop
fin:	mv 	%n s1	
	pop(s2)
	pop(s1)
	pop(s0)
.end_macro

.macro strcmp (%s1, %s2)
	la a0 %s1
	la a1 %s2
	jal strcmp
.end_macro

.macro show_message(%mes, %t)
	push(a0)
	push(a1)
	mv	a0 %mes
	li	a1 %t
	li	a7 55
	ecall
	pop(a1)
	pop(a0)
.end_macro

.eqv READ_ONLY	0	# Открыть для чтения
.eqv WRITE_ONLY	1	# Открыть для записи
.eqv APPEND	    9	# Открыть для добавления
.macro open(%file_name, %opt)
    li   	a7 1024
    mv   	a1 %opt
    la      	a0 %file_name   
    ecall             		
.end_macro

.macro read(%file_descriptor, %strbuf, %size)
    li   a7, 63       	# Системный вызов для чтения из файла
    mv   a0, %file_descriptor       # Дескриптор файла
    la   a1, %strbuf   	# Адрес буфера для читаемого текста
    li   a2, %size 		# Размер читаемой порции
    ecall             	# Чтение
.end_macro

.macro read_addr_reg(%file_descriptor, %reg, %size)
    li   a7, 63       	# Системный вызов для чтения из файла
    mv   a0, %file_descriptor       # Дескриптор файла
    mv   a1, %reg   	# Адрес буфера для читаемого текста из регистра
    li   a2, %size 		# Размер читаемой порции
    ecall             	# Чтение
.end_macro

.macro close(%file_descriptor)
    li   a7, 57       # Системный вызов закрытия файла
    mv   a0, %file_descriptor  # Дескриптор файла
    ecall             # Закрытие файла
.end_macro

.macro allocate(%size)
    li a7, 9
    li a0, %size	# Размер блока памяти
    ecall
.end_macro

