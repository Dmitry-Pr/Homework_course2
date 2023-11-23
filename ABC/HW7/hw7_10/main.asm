# !!! ��������� ������ ����������� �� ���������� ��������.
# !!! ��������30 inst/sec.
.include "macro-syscalls.asm"
.global main
.text
main:	lui     t6 0xffff0          # ���� MMIO (������� ������)
	li	a3 0xffff0010
	li	a4 0xffff0011
        mv      t5 zero             # �������
        mv      t4 zero             # ���������� ��������
        li      t2 20               # ����� ������� �� ���������� ���������
loop:
        mv      t1 zero             # ����� ��������� ������������
        li      t0 1                # ������ ���
        sb      t0 0x12(t6)         # ���������
        lb      t0 0x14(t6)         # �������� ���������
        or      t1 t1 t0            # ��������� ���� � ����� ���������
        li      t0 2                # ������ ���
        sb      t0 0x12(t6)
        lb      t0 0x14(t6)
        or      t1 t1 t0
        li      t0 4                # ������ ���
        sb      t0 0x12(t6)
        lb      t0 0x14(t6)
        or      t1 t1 t0
        li      t0 8                # �������� ���
        sb      t0 0x12(t6)
        lb      t0 0x14(t6)
        or      t1 t1 t0
        beq     t1 t4 same
        li	a0 0xff
        and	a0 a0 t1
        mv	a1 a3
        beqz	a0 clear
        jal get_num
        print_int_dec(a0)
        newline
        jal show_digit		    # ������� ��������� � ���� ������
        j skip		    
clear:	jal clear_digit
skip:	print_int_dec(t5)           # ����� ������������
        print_str(": ")
        print_int_bin(t1)           # ������� ��������� ��� ��������
        print_str(" == ")
        print_int_hex(t1)           # ������ � ��� �����������������
        #mv      a0 t1              # ������� ��������� ��� ��������
        #li      a7 35
        #ecall
        newline
        #li      a0 10
        #li      a7 11
        #ecall
        addi    t5 t5 1             # �������
        mv	a0 t5
        mv	a1 a4
        jal get_num
        jal show_digit              # ������� ��� � ������ ������
        mv      t4 t1
same:   ble     t5 t2 loop

        print_str("20 keystrokes completed\n")

        li      a7 10
        ecall
