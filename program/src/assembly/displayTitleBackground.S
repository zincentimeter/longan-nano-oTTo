.section .data
SCHEME_1: .word 0xf4f6ff
SCHEME_2: .word 0xffcb74
SCHEME_3: .word 0xea907a
SCHEME_4: .word 0x4f8a8b

.section .text
.globl displayTitleBackground
.type displayTitleBackground, @function
displayTitleBackground:
	addi sp, sp, -4				# sp = sp + -4
	sw ra, 0(sp)
	
	# LCD_Clear ( rgb_to_565(SCHEME_1) );
	la a0, SCHEME_1				
	lw a0, 0(a0)
	jal rgb_to_565
	jal LCD_Clear

	# LCD_Fill ( 0, 40, LCD_W-1, LCD_H-1, rgb_to_565(SCHEME_2) );
	la a0, SCHEME_2				
	lw a0, 0(a0)
	jal rgb_to_565
	mv a4, a0					# a4 = rgb_to_565(SCHEME_2)
	li a0, 0					# a0 = 0
	li a1, 40					# a1 = 40
	li a2, 159					# a2 = LCD_W-1
	li a3, 79					# a3 = LCD_H-1
	jal LCD_Fill  				# jump to LCD_Fill and save position to ra
	
	# LCD_Fill ( 0, 60, LCD_W-1, LCD_H-1, rgb_to_565(SCHEME_3) );
	la a0, SCHEME_3				
	lw a0, 0(a0)
	jal rgb_to_565
	mv a4, a0					# a4 = rgb_to_565(SCHEME_3)
	li a0, 0					# a0 = 0
	li a1, 60					# a1 = 60
	li a2, 159					# a2 = LCD_W-1
	li a3, 79					# a3 = LCD_H-1
	jal LCD_Fill  				# jump to LCD_Fill and save position to ra

	# LCD_Fill ( 0, 70, LCD_W-1, LCD_H-1, rgb_to_565(SCHEME_4) );
	la a0, SCHEME_4				
	lw a0, 0(a0)
	jal rgb_to_565
	mv a4, a0					# a4 = rgb_to_565(SCHEME_4)
	li a0, 0					# a0 = 0
	li a1, 70					# a1 = 70
	li a2, 159					# a2 = LCD_W-1
	li a3, 79					# a3 = LCD_H-1
	jal LCD_Fill  				# jump to LCD_Fill and save position to ra

	lw ra, 0(sp)
	addi sp, sp, 4				# sp = sp + 4
	ret