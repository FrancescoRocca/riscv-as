#ifndef ASSEMBLER_INSTRUCTION_H
#define ASSEMBLER_INSTRUCTION_H

#include <stddef.h>
#include <stdint.h>

#define INSTRUCTIONS_COUNT 40
#define INSTRUCTION_NAME_MAX 8

typedef struct instruction {
	char name[INSTRUCTION_NAME_MAX];
	uint8_t opcode;
	uint8_t funct3;
	uint8_t funct7;
} instruction_s;

/* Must be called before using the assembler */
int load_instructions(const char *filename, instruction_s *instructions);

#endif
