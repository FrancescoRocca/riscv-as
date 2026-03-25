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

	FILE *fp = fopen(argv[1], "r");
	if (fp == NULL) {
		printf("Error: unknown file\n");
		return -1;
	}

	// instruction_t instructions[OPCODES_NUM];
	// build_instructions(argv[1], instructions, OPCODES_NUM);

	fclose(fp);

	return EXIT_SUCCESS;
}
