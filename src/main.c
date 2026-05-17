/*  semitov-riscv-as, Small RISC-V Assembler.
	Copyright (C) 2025 SemiTO-V Student Group <semitofive@gmail.com>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>. */

#include "argparser.h"
#include "debug.h"
#include "error.h"
#include "instruction.h"
#include "writer.h"

#include <stdint.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	int exit_code = EXIT_FAILURE;
	uint8_t code[TEXT_SIZE];
	arguments_s *arguments = NULL;

	arguments = argparse(argc, argv);
	if (!arguments) {
		return exit_code;
	}

	if (arguments->file) {
		assembler_error err = assemble_file(arguments->file, code, sizeof(code));
		if (err != ASSEMBLER_OK) {
			log_msg(LOG_ERROR, "assemble_file() failed: %s", assembler_error_str(err));
			goto cleanup;
		}
	}

	if (arguments->elf) {
		const char *filename = "a.out";
		assembler_error err = writer32(filename);
		if (err != ASSEMBLER_OK) {
			log_msg(LOG_ERROR, "writer32() failed: %s", assembler_error_str(err));
			goto cleanup;
		}
	}

	exit_code = EXIT_SUCCESS;

cleanup:
	argparse_free(arguments);
	return exit_code;
}
