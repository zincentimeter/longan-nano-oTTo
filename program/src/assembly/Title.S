.section .text
.globl Title
.type Title, @function
Title:
	addi sp, sp, -4				# sp = sp + -4
	sw ra, 0(sp)
	
	jal displayTitleBackground	# jump to displayTitleBackground
	
	jal displayTitleText		# jump to displayTitleText
	
	lw ra, 0(sp)
	addi sp, sp, 4				# sp = sp + 4
	ret