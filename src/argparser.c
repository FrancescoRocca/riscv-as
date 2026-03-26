#include "argparser.h"
#include <argp.h>
#include <stdlib.h>

const char *argp_program_version = "0.1";
const char doc[] = "SemiTOV-assembler";

static struct argp_option options[] = {
	{"schema", 's', "SCHEMA", 0, "Pass definitions schema"},
	{"compile", 'c', "FILE", 0, "Compile an assembly file"},
	{0},
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
	arguments_s *args = state->input;

	switch (key) {
		case 's': {
			args->definitions_schema = arg;
			break;
		}
		case 'c': {
			args->file = arg;
			break;
		}
		default: {
			return ARGP_ERR_UNKNOWN;
		}
	}
	return 0;
}

static struct argp argp = {options, parse_opt, 0, doc};

arguments_s *argparse(int argc, char **argv) {
	arguments_s *arguments = calloc(1, sizeof *arguments);
	if (!arguments) {
		return NULL;
	}

	/* Default values */
	/* Find "definitions.schema" inside the assets folder */
	arguments->definitions_schema = "assets/definitions.schema";
	arguments->file = "test/base.s";

	argp_parse(&argp, argc, argv, 0, 0, arguments);

	return arguments;
}

void argparse_free(arguments_s *arguments) {
	if (arguments) {
		free(arguments);
	}
}
