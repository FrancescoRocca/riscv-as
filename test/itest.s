addi x1, x0, 10
addi x2, x1, 5
addi x3, x2, -20

slti x4, x2, 20
slti x5, x2, 10

addi x6, x0, 0x5
andi x7, x6, 0xFF
ori  x8, x6, 0x23
xori x9, x6, 0xA0

addi x10, x0, 1
slli x11, x10, 4
srli x12, x11, 2
    
addi x13, x0, -8
srai x14, x13, 1

lw x10, 8(x13) # Test
lh x10, x7 # Test
