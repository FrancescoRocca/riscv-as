#include "argparser.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	arguments_s *arguments = argparse(argc, argv);
	if (!arguments) {
		return EXIT_FAILURE;
	}

	fprintf(stdout, "[info] loading definitions schema from %s ...\n", arguments->definitions_schema);
	fprintf(stdout, "[info] compiling %s ...\n", arguments->file);
	FILE *fp = fopen(arguments->file, "r");
	if (fp == NULL) {
		printf("Error: unknown file\n");
		return -1;
	}

	// instruction_t instructions[OPCODES_NUM];
	// build_instructions(argv[1], instructions, OPCODES_NUM);

	fclose(fp);

	argparse_free(arguments);

	return EXIT_SUCCESS;
}
