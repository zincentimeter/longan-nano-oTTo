# data
.section .data
# color array 1
colors_1:
.word 0xbc4d9b, 0xc24795, 0xc54291, 0xcf3887, 0xd62f7f, 0xdd2777, 0xe41f70, 0xeb1567, 0xf20e60, 0xf7085a
# color array 2
colors_2:
.word 0x01b19b, 0x0db293, 0x1fb588, 0x34b97b, 0x41bb72, 0x50bd69, 0x5abf62, 0x66c15a, 0x78c450, 0x87c747

# text
.section .text
.globl transitionVE
.type transitionVE, @function
# draw some visual effect
transitionVE:
  # require stack
  addi sp, sp, -4
  #save ra
  sw ra, 0(sp)
  #init t0 for i, t2 for 10
  addi t0,zero,0
  addi t2,zero,10
  #init t1 for colors_1[0]
  la t1, colors_1
draw1:
  # draw a layer of rec border
  # prepare argument for rgb_to_565
  lw a0, 0(t1)
  # rgb=>565
  jal rgb_to_565
  # move return value to a4 for LCD_Fill
  add a4,zero,a0
  # a0 <== 72-8i
  # a0 = 8
  addi a0, zero, 8
  # a0 = 8i
  mul a0, a0, t0
  # a0 = 8i-72
  addi a0, a0, -72
  # a0 = 72-8i
  sub a0, zero, a0
  # save a0
  add t3, zero, a0
  # a1 <== 36-4i
  # a1 = 4
  addi a1, zero, 4
  # a1 = 4i
  mul a1, a1, t0
  # a1 = 4i-36
  addi a1, a1, -36
  # a1 = 36-4i
  sub a1, zero, a1
  # save a1
  add t4, zero, a1
  # a2 <== 87+8i
  # a2 = 8
  addi a2, zero, 8
  # a2 = 8i
  mul a2, a2, t0
  # a2 = 8i+87
  addi a2, a2, 87
  add t5, zero, a2
  # a3 <== 39-4i
  # a3 = 4
  addi a3, zero, 4
  # a3 = 4i
  mul a3, a3, t0
  # a3 = 4i-39
  addi a3, a3, -39
  # a3 = 39-4i
  sub a3, zero, a3
  # call LCD_Fill for top border
  jal LCD_Fill
  # recover a0, a2
  add a0, t3, zero
  add a2, t5, zero
  # a1 <== 40+4i
  # a1 = 4
  addi a1, zero, 4
  # a1 = 4i
  mul a1, a1, t0
  # a1 = 4i+40
  addi a1, a1, 40
  # a3 <== 43+4i
  # a3 = 4
  addi a3, zero, 4
  # a3 = 4i
  mul a3, a3, t0
  # a3 = 4i+43
  addi a3, a3, 43
  # save a3
  add t6, a3, zero
  # call LCD_Fill for bottom border
  jal LCD_Fill
  # recover a0, a1, a3
  add a0, t3, zero
  add a1, t4, zero
  add a3, t6, zero
  # a2 <== 79-8i
  # a2 = 8
  addi a2, zero, 8
  # a2 = 8i
  mul a2, a2, t0
  # a2 = 8i-79
  addi a2, a2, -79
  # a2 = 79-8i
  sub a2, zero, a2
  # call LCD_Fill for left border
  jal LCD_Fill
  # recover a1, a2, a3
  add a1, t4, zero
  add a2, t5, zero
  add a3, t6, zero
  # a0 <== 72+8i
  # a0 = 8
  addi a0, zero, 8
  # a0 = 8i
  mul a0, a0, t0
  # a0 = 8i+72
  addi a0, a0, 72
  # call LCD_Fill for bottom border
  jal LCD_Fill

  # delay 100ms for animation
  addi a0, zero, 100
  jal delay_1ms

  # loop if t0+1<10
  addi t0,t0,1
  # from colors_1[i] to colors_1[i+1]
  addi t1,t1,4
  blt t0,t2,draw1

  # draw1 done, draw2 begins
  #init t0 for i, t2 for 10, again
  addi t0,zero,0
  addi t2,zero,10
  #init t1 for colors_1[0]
  la t1, colors_2
draw2:
  # draw a layer of rec
  # prepare argument for rgb_to_565
  lw a0, 0(t1)
  # rgb=>565
  jal rgb_to_565
  # move return value to a4 for LCD_Fill
  add a4,zero,a0
  # a0 <== 8i
  # a0 = 8
  addi a0, zero, 8
  # a0 = 8i
  mul a0, t0, a0
  # a1 <== 4i
  # a1 = 4
  addi a1, zero, 4
  # a1 = 4i
  mul a1, t0, a1
  # a2 <== 159-8i
  # a2 = 8
  addi a2, zero, 8
  # a2 = 8i
  mul a2, a2, t0
  # a2 = 8i-159
  addi a2, a2, -159
  # a2 = 159-8i
  sub a2, zero, a2
  # a3 <== 79-4i
  # a3 = 4
  addi a3, zero, 4
  # a3 = 4i
  mul a3, a3, t0
  # a3 = 4i-79
  addi a3, a3, -79
  # a3 = 79-4i
  sub a3, zero, a3
  jal LCD_Fill

  # delay 100ms for animation
  addi a0, zero, 100
  jal delay_1ms

  # loop if t0+1<10
  addi t0,t0,1
  # from colors_2[i] to colors_2[i+1]
  addi t1,t1,4
  blt t0,t2,draw2

  # recover ra
  lw ra, 0(sp)
  # restore stack
  addi sp, sp, 4
  # return!
  ret