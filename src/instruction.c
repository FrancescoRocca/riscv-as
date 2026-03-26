#include "instruction.h"
#include "common.h"
#include "hash.h"

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

int load_instructions(const char *filename, hashmap_s *hashmap) {
	FILE *fp = fopen(filename, "r");

	if (!fp) {
		fprintf(stderr, "Unable to open file: %s\n", filename);
		return EXIT_FAILURE;
	}

	char name[INSTRUCTION_NAME_MAX];
	char opcode[OPCODE_MAX];

	instruction_s instruction = {0};
	while ((fscanf(fp, " %s %s %hhx %hhx ", name, opcode, &instruction.funct3, &instruction.funct7) != EOF)) {
		instruction.opcode = format_binary(opcode, OPCODE_MAX);
		// fprintf(stdout, "[debug] %s %02x %02x %02x\n", name, format_binary(opcode, OPCODE_MAX), instruction.funct3,
		// instruction.funct7);

		/* Insert the new pair into hashmap */
		int ret = hm_set(hashmap, name, instruction);
		if (ret != 0) {
			return EXIT_FAILURE;
		}
	}

	fclose(fp);

	return EXIT_SUCCESS;
}
