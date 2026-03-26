#ifndef ASSEMBLER_COMMON_H
#define ASSEMBLER_COMMON_H

#include <stdint.h>

#define INSTRUCTIONS_COUNT 40

typedef struct instruction {
	uint8_t opcode;
	uint8_t funct3;
	uint8_t funct7;
} instruction_s;

#endif
