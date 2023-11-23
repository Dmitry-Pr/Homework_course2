# !!! Программа должна выполняться на пониженной скорости.
# !!! Примерно30 inst/sec.
.include "macro-syscalls.asm"
.global main
.text
main:	lui     t6 0xffff0          # база MMIO (старшие адреса)
	li	a3 0xffff0010
	li	a4 0xffff0011
        mv      t5 zero             # счётчик
        mv      t4 zero             # предыдущее значение
        li      t2 20               # Число нажатий до завершения программы
loop:
        mv      t1 zero             # общий результат сканирования
        li      t0 1                # первый ряд
        sb      t0 0x12(t6)         # сканируем
        lb      t0 0x14(t6)         # забираем результат
        or      t1 t1 t0            # добавляем биты в общий результат
        li      t0 2                # второй ряд
        sb      t0 0x12(t6)
        lb      t0 0x14(t6)
        or      t1 t1 t0
        li      t0 4                # третий ряд
        sb      t0 0x12(t6)
        lb      t0 0x14(t6)
        or      t1 t1 t0
        li      t0 8                # четвёртый ряд
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
        jal show_digit		    # запишем результат в биты окошка
        j skip		    
clear:	jal clear_digit
skip:	print_int_dec(t5)           # Номер срабатывания
        print_str(": ")
        print_int_bin(t1)           # выведем результат как двоичное
        print_str(" == ")
        print_int_hex(t1)           # Заодно и как шестнадцатиричное
        #mv      a0 t1              # выведем результат как двоичное
        #li      a7 35
        #ecall
        newline
        #li      a0 10
        #li      a7 11
        #ecall
        addi    t5 t5 1             # счётчик
        mv	a0 t5
        mv	a1 a4
        jal get_num
        jal show_digit              # запишем его в другое окошко
        mv      t4 t1
same:   ble     t5 t2 loop

        print_str("20 keystrokes completed\n")

        li      a7 10
        ecall
