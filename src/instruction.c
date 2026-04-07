#include "instruction.h"

#include "hash.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOAD_STORE_MAX 4

/* Return 1 (true) if the opcode is a load/store one */
static int is_load_store(char *buf) {
    if( ! strncmp("lb",buf, LOAD_STORE_MAX) )
        return 1;
	else if (!strncmp("lh", buf, LOAD_STORE_MAX))
        return 1;
	else if (!strncmp("lw", buf, LOAD_STORE_MAX))
        return 1;
	else if (!strncmp("lbu", buf, LOAD_STORE_MAX))
        return 1;
	else if (!strncmp("lhu", buf, LOAD_STORE_MAX))
        return 1;
	else if (!strncmp("sb", buf, LOAD_STORE_MAX))
        return 1;
	else if (!strncmp("sh", buf, LOAD_STORE_MAX))
        return 1;
	else if (!strncmp("sw", buf, LOAD_STORE_MAX))
        return 1;

    return 0;
}

static uint8_t get_register(char *reg) {
	uint8_t regValue = 0x0;

	switch (reg[0]) {
		case 'x':
			sscanf(&reg[1], "%hhu", &regValue);
			break;
		case 'a':
			/* a0-a7 -> x10-x17*/
			regValue = (reg[1] - '0') + 10;
			break;
		case 's':
			if (reg[1] == 'p') {
				regValue = 0x2;
			} else {
				uint8_t t = 0x0;
				sscanf(&reg[1], "%hhu", &t);
				/* s0/s1 -> x8/x9 */
				/* s2-11 -> x18-27 */
				regValue = t < 2 ? t + 8 : t + 16;
			}
			break;
		case 't':
			/* tp -> x4 */
			if (reg[1] == 'p') {
				regValue = 0x4;
			} else {
				uint8_t t = reg[1] - '0';
				/* t0-2 -> x5-7 */
				/* t3-6 -> x28-31 */
				regValue = t < 3 ? t + 5 : t + 25;
			}
			break;
		case 'r':
			/* ra (return address) reg (x1)*/
			regValue = 0x1;
			break;
		case 'g':
			/* gp -> x3 */
			regValue = 0x3;
			break;
		default:
			break;
	}

	return regValue;
}

int assemble_file(const char *filename) {
	FILE *fp = fopen(filename, "r");
	if (!fp) {
		return EXIT_FAILURE;
	}

	char name[NAME_LEN];
	char rd[REGISTER_LEN], rs1[REGISTER_LEN], rs2[REGISTER_LEN];
	int16_t imm, imm2;
	int32_t imm32;
	int32_t res = 0x0;
	const instruction_s *instr;
    char lineBuf[512];
	// while ((fscanf(fp, " %s ", name) != EOF)) {
	while ( (fgets(lineBuf, 512 , fp)) ) {
        sscanf(lineBuf, " %s ", name);
		instr = find_instruction(name, strlen(name));
		if (instr == NULL) {
			fprintf(stderr, "[error] unknown instruction:\n%s\n", name);
			break;
		}

		fprintf(stdout, "[info] fetching instruction: %s (%c TYPE)\n", name, instr->type);

		switch (instr->type) {
			case R_TYPE:
				sscanf(lineBuf, "%*s %3[^,], %3[^,], %3[^#\n]", rd, rs1, rs2);
				printf("R-Type Read: %s %s %s\n", rd, rs1, rs2);
				res = (instr->funct7 << 25) | (get_register(rs2) << 20) | (get_register(rs1) << 15) |
					  (instr->funct3 << 12) | (get_register(rd) << 7) | instr->opcode;
				printf("Write: %08x\n", res);
				// printf("Write: %02x %02x %02x %02x %02x %02x \n", instr->funct7, get_register(rs2),
				// get_register(rs1), 	   instr->funct3, 	   get_register(rd), instr->opcode);
				break;
			case I_TYPE:
                /* Load opcodes have a specific syntax opcode, reg, offset(reg) */
				if (is_load_store(name)) {

					size_t res = sscanf(lineBuf, "%*s %3[^,], %hi(%3[^)])", rd, &imm, rs1);
                    /* Probably an implicit zero offset immediate */
					if (res < 3) {
                        sscanf(lineBuf, "%*s, %3[^,], %3[^\n#]", rd, rs1);
                        imm = 0x0; // Still needed this immediate
					}
				}
				else
					sscanf(lineBuf, "%*s %3[^,], %3[^,], %hi[^#\n] ", rd, rs1, &imm);

				printf("I-Type Read: %s, %s, %hi\n", rd, rs1, imm);
                res = imm << 20 | (get_register(rs1) << 15) | (instr->funct3 << 12) | (get_register(rd) << 7) | instr->opcode;
                printf("Write: %08x\n", res);
				// printf("Write: %02x %02x %02x %02x %02x %02x\n", instr->opcode, instr->funct3, instr->funct7,
				//	   get_register(rd), get_register(rs1), imm);
                break;

			case S_TYPE:
                /* Store opcodes have a specific syntax opcode, src, offset(dest) */
				if (is_load_store(name)) {
					size_t res = sscanf(lineBuf, "%*s %3[^,], %hi(%3[^)])", rs1, &imm, rd);
                    /* Probably an implicit zero offset immediate */
					if (res < 3) {
                        sscanf(lineBuf, "%*s, %3[^,], %3[^\n#]", rd, rs1);
                        imm = 0x0;
					}
				}
				else
                    sscanf(lineBuf, "%*s %hi, %3[^,], %3[^,], %hi[^#\n] ", &imm, rs1, rs2, &imm2);

				printf("S-Type Read: %hi, %s, %s, %hi\n", imm, rd, rs1, imm2);
				res = (imm2 << 25) | (get_register(rs2) << 20) | (get_register(rs1) << 15) | (instr->funct3 << 12) |
					  (imm << 7) | instr->opcode;
				printf("Write: %08x\n", res);
				//	   printf("Write: %02x %02x %02x %02x %02x %02x %02x \n", instr->opcode, instr->funct3,
				//instr->funct7, imm, 	   get_register(rd), get_register(rs1), imm2);
				break;
			case B_TYPE:
				sscanf(lineBuf, "%*s %hi, %3[^,], %3[^,], %hi[^#\n] ", &imm, rs1, rs2, &imm2);
				printf("B-Type Read: %hi, %s, %s, %hi\n", imm, rd, rs1, imm2);
				res = (imm2 << 25) | (get_register(rs2) << 20) | (get_register(rs1) << 15) | (instr->funct3 << 12) |
					  (imm << 7) | instr->opcode;
				printf("Write: %08x\n", res);
				// printf("Write: %02x %02x %02x %02x %02x %02x %02x \n", instr->opcode, instr->funct3, instr->funct7,
				// imm, 	   get_register(rd), get_register(rs1), imm2);
				break;
			case U_TYPE:
				sscanf(lineBuf, "%*s %3[^,], %i[^#\n] ", rd, &imm32);
				printf("U-Type Read: %s, %i\n", rd, imm32);
				res = (imm32 << 12) | (get_register(rd) << 7) | instr->opcode;
				printf("Write: %08x\n", res);
				// printf("Write: %02x %02x %02x %02x %02x", instr->opcode, instr->funct3, instr->funct7,
				// get_register(rd), 	   imm32);
				break;
			case J_TYPE:
				sscanf(lineBuf, "%*s %3[^,], %i[^#\n] ", rd, &imm32);
				printf("J-Type Read: %s, %i\n", rd, imm32);
				// TODO Write correctly imm bytes order
				res = (imm << 12) | (get_register(rd) << 7) | instr->opcode;
				printf("Write: %08x\n", res);
				//printf("Write: %02x %02x %02x %02x %02x\n", instr->opcode, instr->funct3, instr->funct7,
				//	   get_register(rd), imm32);
				break;
			default:
				break;
		}
	}

	fclose(fp);

	return EXIT_SUCCESS;
}
