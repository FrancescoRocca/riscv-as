#include "argparser.h"
#include "debug.h"
#include "error.h"
#include "instruction.h"

#include <stdint.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	int exit_code = EXIT_FAILURE;
	arguments_s *arguments = NULL;

	arguments = argparse(argc, argv);
	if (!arguments) {
		return EXIT_FAILURE;
	}

	log_msg(LOG_INFO, "compiling %s ...", arguments->file);

	assembler_error err = assemble_file(arguments->file);
	if (err != ASSEMBLER_OK) {
		log_msg(LOG_ERROR, "assemble_file() failed: %s", assembler_error_str(err));
		goto cleanup;
	}

	exit_code = EXIT_SUCCESS;

cleanup:
	argparse_free(arguments);
	return exit_code;
}
