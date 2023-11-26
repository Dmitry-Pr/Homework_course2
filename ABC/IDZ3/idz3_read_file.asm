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
    	allocate(TEXT_SIZE)		# Результат хранится в a0
    	mv 		s3, a0			# Сохранение адреса кучи в регистре
    	mv 		s5, a0			# Сохранение изменяемого адреса кучи в регистре
    	li		s4, TEXT_SIZE	# Сохранение константы для обработки
    	mv		s6, zero		# Установка начальной длины прочитанного текста
    ###############################################################
read_loop:
    # Чтение информации из открытого файла
    read_addr_reg(s0, s5, TEXT_SIZE) # чтение для адреса блока из регистра
    # Проверка на корректное чтение
    beq		a0 s1 er_read	# Ошибка чтения
    mv   	s2 a0       	# Сохранение длины текста
    add 	s6, s6, s2		# Размер текста увеличивается на прочитанную порцию
    # При длине прочитанного текста меньшей, чем размер буфера,
    # необходимо завершить процесс.
    bne		s2 s4 end_loop
    # Иначе расширить буфер и повторить
    allocate(TEXT_SIZE)		# Результат здесь не нужен, но если нужно то...
    add		s5 s5 s2		# Адрес для чтения смещается на размер порции
    b read_loop				# Обработка следующей порции текста из файла
end_loop:
    # Закрытие файла
    close(s0)
    # Установка нуля в конце прочитанной строки
    ###la	t0 strbuf	 # Адрес начала буфера
    mv	t0 s3		# Адрес буфера в куче
    add t0 t0 s6	# Адрес последнего прочитанного символа
    addi t0 t0 1	# Место для нуля
    sb	zero (t0)	# Запись нуля в конец текста
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
