.include "macrolib.asm"
.global read_file
.eqv    NAME_SIZE 256
.eqv    TEXT_SIZE 512
.data
fmsg:	.asciz "Incorrect read operation\n. Programm is stopped.\n"
.text
read_file:
	push(ra)
	push(s0)
	push(s1)
	push(s2)
	push(s3)
	push(s4)
	push(s5)
	push(s6)
   	mv   	s0 a0
    	allocate(TEXT_SIZE)		# ��������� �������� � a0
    	mv 		s3, a0			# ���������� ������ ���� � ��������
    	mv 		s5, a0			# ���������� ����������� ������ ���� � ��������
    	li		s4, TEXT_SIZE	# ���������� ��������� ��� ���������
    	mv		s6, zero		# ��������� ��������� ����� ������������ ������
    ###############################################################
read_loop:
    # ������ ���������� �� ��������� �����
    read_addr_reg(s0, s5, TEXT_SIZE) # ������ ��� ������ ����� �� ��������
    # �������� �� ���������� ������
    beq		a0 s1 er_read	# ������ ������
    mv   	s2 a0       	# ���������� ����� ������
    add 	s6, s6, s2		# ������ ������ ������������� �� ����������� ������
    # ��� ����� ������������ ������ �������, ��� ������ ������,
    # ���������� ��������� �������.
    bne		s2 s4 end_loop
    # ����� ��������� ����� � ���������
    allocate(TEXT_SIZE)		# ��������� ����� �� �����, �� ���� ����� ��...
    add		s5 s5 s2		# ����� ��� ������ ��������� �� ������ ������
    b read_loop				# ��������� ��������� ������ ������ �� �����
end_loop:
    # �������� �����
    close(s0)
    # ��������� ���� � ����� ����������� ������
    ###la	t0 strbuf	 # ����� ������ ������
    mv	t0 s3		# ����� ������ � ����
    add t0 t0 s6	# ����� ���������� ������������ �������
    addi t0 t0 1	# ����� ��� ����
    sb	zero (t0)	# ������ ���� � ����� ������
    mv		a0 s3	# a0 - result string
    li		a1 0
    j	fin
er_read:
	li 	a1 1
	la	a0 fmsg
	show_message(a0, 1)
fin:	pop(s6)
	pop(s5)
	pop(s4)
	pop(s3)
	pop(s2)
	pop(s1)
	pop(s0)
	pop(ra)
	ret
