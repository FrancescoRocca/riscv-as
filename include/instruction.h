#ifndef ASSEMBLER_INSTRUCTION_H
#define ASSEMBLER_INSTRUCTION_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define OPCODES_NUM 40

typedef struct instruction {
	char name[8];
	uint8_t opcode;
	uint8_t funct3;
	uint8_t funct7;
} instruction_s;

/* Must be called before using the assembler */
int build_instructions(const char *filename, instruction_s *instructions, size_t num);

#endif
