#ifndef ASSEMBLER_INSTRUCTION_H
#define ASSEMBLER_INSTRUCTION_H

#include <stdint.h>

typedef struct instruction {
	char *name;
	uint8_t opcode;
	uint8_t funct3;
	uint8_t funct7;
	char type;
} instruction_s;

/**
 * @brief Assemble the file.
 *
 * @param filename File to assemble.
 */
int assemble_file(const char *filename);

#endif
