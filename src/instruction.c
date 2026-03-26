#include "instruction.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define OPCODE_MAX 8

static uint8_t format_binary(char *s, size_t bits) {
	uint8_t binValue = 0;
	for (int i = 0; i < bits; i++) {
		binValue = (binValue << 1) | (s[i] - '0');
	}

	return binValue;
}

int load_instructions(const char *filename, instruction_s *instructions) {
	FILE *fp = fopen(filename, "r");

	if (!fp) {
		fprintf(stderr, "Unable to open file: %s\n", filename);
		return EXIT_FAILURE;
	}

	char name[INSTRUCTION_NAME_MAX];
	char opcode[OPCODE_MAX];
	uint8_t func3, func7;

	while ((fscanf(fp, " %s %s %hhx %hhx ", name, opcode, &func3, &func7) != EOF)) {
		fprintf(stdout, "[debug] %s %02x %02x %02x\n", name, format_binary(opcode, OPCODE_MAX), func3, func7);
	}

	fclose(fp);

	return EXIT_SUCCESS;
}
