.section .data
RGB_BLACK: .word 0x000000
RGB_WHITE: .word 0xFFFFFF
THIC:      .word 6
WHITE_565: .word 0xFFFF
.section .text
.globl displayTitleText
.type displayTitleText, @function
displayTitleText:
	addi sp, sp, -28			# sp = sp + -28
	sw ra, 0(sp)
	sw s1, 4(sp)				# whiteLoop condition
	sw s2, 8(sp)				# whileLoop iteration
	sw s3, 12(sp)				# THIC
	sw s4, 16(sp)				# 
	sw s5, 20(sp)				# size
	sw s6, 24(sp)				# color
	
	la s6, RGB_BLACK			# color = RGB_BLACK;
	lw s6, 0(s6)
	
infiniteLoop:					# while(1)
	li a0, 20					# a0 = 20
	jal delay_1ms				# delay_1ms(20);
	la s6, RGB_BLACK			# color = RGB_BLACK;
	lw s6, 0(s6)
	jal detectBoot0				# detectBoot0();

	la s1, RGB_WHITE			# whiteLoop condition = RGB_BLACK;
	lw s1, 0(s1)
	li s2, 0x010101				# whileLoop iteration = 0x010101;
	whiteLoop:
		bgt s6, s1, whiteLoopEnd 	# if s6 > s1 then whiteLoopEnd
		jal detectBoot0;			# detectBoot0();

		# CAPITAL O
		# x_1 = y_1 = 10;
		# size = 30;
		# LCD_Fill( x_1, y_1, x_1 + size  , y_1 + size , rgb_to_565(color) );
		mv a0, s6
		jal rgb_to_565  			# jump to rgb_to_565 and save position to ra
		mv a4, a0 					# a4 = rgb_to_565(color)
		li a0, 10 					# a0 = 10
		li a1, 10 					# a1 = 10
		addi a2, a0, 30; 			# a2 = a0 + 30
		addi a3, a1, 30; 			# a3 = a1 + 30
		jal LCD_Fill
		# LCD_Fill( x_1+THIC, y_1+THIC, x_1 + size -THIC, y_1 + size -THIC , WHITE);
		la s3, THIC
		lw s3, 0(s3)
		addi a0, s3, 10; 			# a0 = s3 + 10
		addi a1, s3, 10; 			# a1 = s3 + 10
		li a2, 40					# a2 = 40
		sub a2, a2, s3 				# a2 = a2 - s3
		li a3, 40 					# a3 = 40
		sub a3, a3, s3 				# a3 = a3 - s3
		la a4, WHITE_565			# a4 = WHITE
		lw a4, 0(a4)
		jal LCD_Fill 				# jump to LCD_Fill and save position to ra
		
		# Under `T`
		# x_2 = 45; y_2 = 20;
		# LCD_Fill( x_2, y_2, x_2 + 25, y_2 + THIC , RGB_WHITE - rgb_to_565(color) );
		mv a0, s6
		jal rgb_to_565  			# jump to rgb_to_565 and save position to ra
		la a4, RGB_WHITE
		lw a4, 0(a4)
		sub a4, a4, a0				# a4 = a4 - a0
		li a0, 45 					# a0 = 45
		li a1, 20 					# a1 = 20
		addi a2, a0, 25; 			# a2 = a0 + 25
		add a3, a1, s3; 			# a3 = a1 + s3
		jal LCD_Fill
		# LCD_Fill( x_2 + 13 - THIC/2, y_2 , x_2 + 13 + THIC/2, y_2 + 40, RGB_WHITE - rgb_to_565(color));
		mv a0, s6
		jal rgb_to_565  			# jump to rgb_to_565 and save position to ra
		la a4, RGB_WHITE
		lw a4, 0(a4)
		sub a4, a4, a0				# a4 = a4 - a0
		li a0, 55 					# a0 = 55
		li a1, 20 					# a1 = 20
		add a2, a0, s3; 			# a2 = a0 + s3
		addi a3, a1, 40; 			# a3 = a1 + 40
		jal LCD_Fill

		# Under `T`
		# x_2 = 75; y_2 = 20;
		# LCD_Fill( x_2, y_2, x_2 + 25, y_2 + THIC , RGB_WHITE - rgb_to_565(color) );
		# LCD_Fill( x_2 + 13 - THIC/2, y_2 , x_2 + 13 + THIC/2, y_2 + 40, RGB_WHITE - rgb_to_565(color));
		mv a0, s6
		jal rgb_to_565  			# jump to rgb_to_565 and save position to ra
		la a4, RGB_WHITE
		lw a4, 0(a4)
		sub a4, a4, a0				# a4 = a4 - a0
		li a0, 75 					# a0 = 75
		li a1, 20 					# a1 = 20
		addi a2, a0, 25; 			# a2 = a0 + 25
		add a3, a1, s3; 			# a3 = a1 + s3
		jal LCD_Fill
		# LCD_Fill( x_2 + 13 - THIC/2, y_2 , x_2 + 13 + THIC/2, y_2 + 40, RGB_WHITE - rgb_to_565(color));
		mv a0, s6
		jal rgb_to_565  			# jump to rgb_to_565 and save position to ra
		la a4, RGB_WHITE
		lw a4, 0(a4)
		sub a4, a4, a0				# a4 = a4 - a0
		li a0, 85 					# a0 = 85
		li a1, 20 					# a1 = 20
		add a2, a0, s3; 			# a2 = a0 + s3
		addi a3, a1, 40; 			# a3 = a1 + 40
		jal LCD_Fill

		# SMALL O
		# x_1 = 105; y_1 = 20;
		# size = 20;
		# LCD_Fill( x_1, y_1, x_1 + size  , y_1 + size , rgb_to_565(color) );
		# LCD_Fill( x_1+THIC, y_1+THIC, x_1 + size -THIC, y_1 + size -THIC , WHITE);
		mv a0, s6
		jal rgb_to_565  			# jump to rgb_to_565 and save position to ra
		mv a4, a0 					# a4 = rgb_to_565(color)
		li a0, 105 					# a0 = 105
		li a1, 20 					# a1 = 20
		addi a2, a0, 20; 			# a2 = a0 + 20
		addi a3, a1, 20; 			# a3 = a1 + 20
		jal LCD_Fill
		# LCD_Fill( x_1+THIC, y_1+THIC, x_1 + size -THIC, y_1 + size -THIC , WHITE);
		la s3, THIC
		lw s3, 0(s3)
		addi a0, s3, 105; 			# a0 = s3 + 105
		addi a1, s3, 20; 			# a1 = s3 + 20
		li a2, 125					# a2 = 125
		sub a2, a2, s3 				# a2 = a2 - s3
		li a3, 40 					# a3 = 40
		sub a3, a3, s3 				# a3 = a3 - s3
		la a4, WHITE_565			# a4 = WHITE
		lw a4, 0(a4)
		jal LCD_Fill 				# jump to LCD_Fill and save position to ra

		add s6, s6, s2; 			# s6 = s6 + 0x010101
		j whiteLoop
	whiteLoopEnd:

	li a0, 20					# a0 = 20
	jal delay_1ms				# delay_1ms(20);
	la s6, RGB_BLACK			# color = RGB_BLACK;
	lw s6, 0(s6)
	jal detectBoot0				# detectBoot0();

	la s1, RGB_WHITE			# whiteLoop condition = RGB_BLACK;
	lw s1, 0(s1)
	li s2, 0x010101				# whileLoop iteration = 0x010101;
	whiteLoop2:
		bgt s6, s1, whiteLoopEnd2 	# if s6 > s1 then whiteLoopEnd
		jal detectBoot0;			# detectBoot0();

		# CAPITAL O
		# x_1 = y_1 = 10;
		# size = 30;
		# LCD_Fill( x_1, y_1, x_1 + size  , y_1 + size , RGB_WHITE - rgb_to_565(color) );
		mv a0, s6
		jal rgb_to_565  			# jump to rgb_to_565 and save position to ra
		la a4, RGB_WHITE
		lw a4, 0(a4)
		sub a4, a4, a0				# a4 = a4 - a0

		li a0, 10 					# a0 = 10
		li a1, 10 					# a1 = 10
		addi a2, a0, 30; 			# a2 = a0 + 30
		addi a3, a1, 30; 			# a3 = a1 + 30
		jal LCD_Fill
		# LCD_Fill( x_1+THIC, y_1+THIC, x_1 + size -THIC, y_1 + size -THIC , WHITE);
		la s3, THIC
		lw s3, 0(s3)
		addi a0, s3, 10; 			# a0 = s3 + 10
		addi a1, s3, 10; 			# a1 = s3 + 10
		li a2, 40					# a2 = 40
		sub a2, a2, s3 				# a2 = a2 - s3
		li a3, 40 					# a3 = 40
		sub a3, a3, s3 				# a3 = a3 - s3
		la a4, WHITE_565			# a4 = WHITE
		lw a4, 0(a4)
		jal LCD_Fill 				# jump to LCD_Fill and save position to ra
		
		# Under `T`
		# x_2 = 45; y_2 = 20;
		# LCD_Fill( x_2, y_2, x_2 + 25, y_2 + THIC , rgb_to_565(color) );
		mv a0, s6
		jal rgb_to_565  			# jump to rgb_to_565 and save position to ra
		mv a4, a0 					# a4 = rgb_to_565(color)

		li a0, 45 					# a0 = 45
		li a1, 20 					# a1 = 20
		addi a2, a0, 25; 			# a2 = a0 + 25
		add a3, a1, s3; 			# a3 = a1 + s3
		jal LCD_Fill
		# LCD_Fill( x_2 + 13 - THIC/2, y_2 , x_2 + 13 + THIC/2, y_2 + 40, rgb_to_565(color));
		mv a0, s6
		jal rgb_to_565  			# jump to rgb_to_565 and save position to ra
		mv a4, a0 					# a4 = rgb_to_565(color)

		li a0, 55 					# a0 = 55
		li a1, 20 					# a1 = 20
		add a2, a0, s3; 			# a2 = a0 + s3
		addi a3, a1, 40; 			# a3 = a1 + 40
		jal LCD_Fill

		# Under `T`
		# x_2 = 75; y_2 = 20;
		# LCD_Fill( x_2, y_2, x_2 + 25, y_2 + THIC , rgb_to_565(color) );
		mv a0, s6
		jal rgb_to_565  			# jump to rgb_to_565 and save position to ra
		mv a4, a0 					# a4 = rgb_to_565(color)

		li a0, 75 					# a0 = 75
		li a1, 20 					# a1 = 20
		addi a2, a0, 25; 			# a2 = a0 + 25
		add a3, a1, s3; 			# a3 = a1 + s3
		jal LCD_Fill
		# LCD_Fill( x_2 + 13 - THIC/2, y_2 , x_2 + 13 + THIC/2, y_2 + 40, rgb_to_565(color));
		mv a0, s6
		jal rgb_to_565  			# jump to rgb_to_565 and save position to ra
		mv a4, a0 					# a4 = rgb_to_565(color)

		li a0, 85 					# a0 = 85
		li a1, 20 					# a1 = 20
		add a2, a0, s3; 			# a2 = a0 + s3
		addi a3, a1, 40; 			# a3 = a1 + 40
		jal LCD_Fill

		# SMALL O
		# x_1 = 105; y_1 = 20;
		# size = 20;
		# LCD_Fill( x_1, y_1, x_1 + size  , y_1 + size , RGB_WHITE - rgb_to_565(color) );
		# LCD_Fill( x_1+THIC, y_1+THIC, x_1 + size -THIC, y_1 + size -THIC , WHITE);
		mv a0, s6
		jal rgb_to_565  			# jump to rgb_to_565 and save position to ra
		la a4, RGB_WHITE
		lw a4, 0(a4)
		sub a4, a4, a0				# a4 = a4 - a0
			
		li a0, 105 					# a0 = 105
		li a1, 20 					# a1 = 20
		addi a2, a0, 20; 			# a2 = a0 + 20
		addi a3, a1, 20; 			# a3 = a1 + 20
		jal LCD_Fill
		# LCD_Fill( x_1+THIC, y_1+THIC, x_1 + size -THIC, y_1 + size -THIC , WHITE);
		la s3, THIC
		lw s3, 0(s3)
		addi a0, s3, 105; 			# a0 = s3 + 105
		addi a1, s3, 20; 			# a1 = s3 + 20
		li a2, 125					# a2 = 125
		sub a2, a2, s3 				# a2 = a2 - s3
		li a3, 40 					# a3 = 40
		sub a3, a3, s3 				# a3 = a3 - s3
		la a4, WHITE_565			# a4 = WHITE
		lw a4, 0(a4)
		jal LCD_Fill 				# jump to LCD_Fill and save position to ra
		add s6, s6, s2; 			# s6 = s6 + 0x010101
		j whiteLoop2
	whiteLoopEnd2:

	j infiniteLoop
	
	lw ra, 0(sp)
	lw s1, 4(sp)				# x_1
	lw s2, 8(sp)				# y_1
	lw s3, 12(sp)				# x_2
	lw s4, 16(sp)				# y_2
	lw s5, 20(sp)				# size
	lw s6, 24(sp)				# color
	addi sp, sp, 28			# sp = sp + -28
	ret