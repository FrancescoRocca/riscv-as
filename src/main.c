#include "argparser.h"
#include "instruction.h"

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

	/* Load definitions into instructions struct */
	instruction_s instructions[INSTRUCTIONS_COUNT];
	load_instructions(arguments->definitions_schema, instructions);

	fclose(fp);

	argparse_free(arguments);

	return EXIT_SUCCESS;
}
