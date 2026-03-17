#include "hash.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 256
#define KEYWORD_SIZE 6

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stdout, "Usage: %s <FILE.s>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	char line[BUF_SIZE];
	// Open file
	char opcode[KEYWORD_SIZE];
	// char reg1[KEYWORD_SIZE];
	// char reg2[KEYWORD_SIZE];
	// char reg3[KEYWORD_SIZE];

	uint8_t opcodes[OPCODES_NUM * 4];
	for (int i = 0; i < OPCODES_NUM; i++) {
		opcodes[i] = 0;
	}
	FILE *f = fopen(argv[1], "r");
	if (f == NULL) {
		printf("Error: no file found\n");
		return -1;
	}
	while (fgets(line, sizeof(line), f)) {
		// sscanf(line,"%s %[^,], %[^,], %[^,]\n",opcode, reg1, reg2, reg3);
		sscanf(line, "%s\n", opcode);
		// printf("Opcode: %s Hash: %d\n", opcode, (sdbm(opcode) % 64));
		opcodes[sdbm(opcode) % (OPCODES_NUM * 2)]++;
	}
	for (int i = 0; i < OPCODES_NUM * 2; i++) {
		printf("Index: %d Occurencies: %d \n", i, opcodes[i]);
	}
	// Read each line
	// Parse each line
	// Write output file
	fclose(f);

	return EXIT_SUCCESS;
}
