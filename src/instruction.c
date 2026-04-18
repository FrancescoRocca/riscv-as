#include "instruction.h"

#include "debug.h"
#include "hash.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* S-Type Helpers */
#define GET_IMM_0_4(imm) (imm & 0x1F )
#define GET_IMM_5_11(imm) (imm & 0xFE0 )
// Get from assembled instruction
#define GET_FI_IMM_0_4(instr) ( instr & 0xF80 )
#define GET_FI_IMM_5_11(instr) ( instr & 0x7F000000 )
#define GET_FI_FUNCT3(instr) ( instr &  0x7000 )
#define GET_FI_RS1(instr) (instr & 0xf8000 )
#define GET_FI_RS2(instr) (instr & 0x1f00000 )
/* Return 1 if the string is made of only whitespaces */
static int is_only_ws(const char* str){
    while(*str!='\0'){
        if( !isspace(*str) )
            return 0;
        str++;
    }
    return 1;
}

/* Return 1 (true) if the opcode is a load/store one */
static int is_load_store(const char *op) {
	if (!op) {
		return 0;
	}

	return strcmp(op, "lb") == 0 || strcmp(op, "lh") == 0 || strcmp(op, "lw") == 0 || strcmp(op, "lbu") == 0 ||
		   strcmp(op, "lhu") == 0 || strcmp(op, "sb") == 0 || strcmp(op, "sh") == 0 || strcmp(op, "sw") == 0;
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
static int32_t get_opcode(const instruction_s *instr, char *lineBuf, char *name){
    char rd[REGISTER_LEN], rs1[REGISTER_LEN], rs2[REGISTER_LEN];
	int16_t imm=0x0, imm2=0x0;
	int32_t imm32=0x0;
    int32_t res = 0x0;
    switch (instr->type) {
				case R_TYPE:
					sscanf(lineBuf, "%*s %3[^,], %3[^,], %3[^#\n]", rd, rs1, rs2);
					log_msg(LOG_DEBUG, "R-Type Read: %s %s %s", rd, rs1, rs2);
					res = (instr->funct7 << 25) | (get_register(rs2) << 20) | (get_register(rs1) << 15) |
						  (instr->funct3 << 12) | (get_register(rd) << 7) | instr->opcode;
					log_msg(LOG_DEBUG, "Write: %08x", res);
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
					} else {
						sscanf(lineBuf, "%*s %3[^,], %3[^,], %hi[^#\n] ", rd, rs1, &imm);
					}

					log_msg(LOG_DEBUG, "I-Type Read: %s, %s, %hi", rd, rs1, imm);
					res = imm << 20 | (get_register(rs1) << 15) | (instr->funct3 << 12) | (get_register(rd) << 7) |
						  instr->opcode;
					log_msg(LOG_DEBUG, "Write: %08x", res);
					break;

				case S_TYPE:
					/* Store opcodes have a specific syntax opcode, src, offset(dest) */
					res = sscanf(lineBuf, "%*s %3[^,], %hi(%3[^)])", rs2, &imm, rs1);
					/* Assuming an implicit zero offset immediate */
					if (res < 3) {
						sscanf(lineBuf, "%*s, %3[^,], %3[^\n#]", rs1, rs2);
						imm = 0x0;
					}
					log_msg(LOG_DEBUG, "S-Type Read: %hi, %s, %s", imm, rs1, rs2);
					res = ( GET_IMM_5_11(imm) << 25) | (get_register(rs2) << 20) | (get_register(rs1) << 15) | (instr->funct3 << 12) |
						  ( GET_IMM_0_4(imm) << 7) | instr->opcode;
					log_msg(LOG_DEBUG, "Write: %08x", res);
					break;
				case B_TYPE:
					sscanf(lineBuf, "%*s %hi, %3[^,], %3[^,], %hi[^#\n] ", &imm, rs1, rs2, &imm2);
					log_msg(LOG_DEBUG, "B-Type Read: %hi, %s, %s, %hi", imm, rd, rs1, imm2);
					res = (imm2 << 25) | (get_register(rs2) << 20) | (get_register(rs1) << 15) | (instr->funct3 << 12) |
						  (imm << 7) | instr->opcode;
					log_msg(LOG_DEBUG, "Write: %08x", res);
					break;
				case U_TYPE:
					sscanf(lineBuf, "%*s %3[^,], %i[^#\n] ", rd, &imm32);
					log_msg(LOG_DEBUG, "U-Type Read: %s, %i", rd, imm32);
					res = (imm32 << 12) | (get_register(rd) << 7) | instr->opcode;
					log_msg(LOG_DEBUG, "Write: %08x", res);
					break;
				case J_TYPE:
					sscanf(lineBuf, "%*s %3[^,], %i[^#\n] ", rd, &imm32);
					log_msg(LOG_DEBUG, "J-Type Read: %s, %i", rd, imm32);
					// TODO Write correctly imm bytes order
					res = (imm << 12) | (get_register(rd) << 7) | instr->opcode;
					log_msg(LOG_DEBUG, "Write: %08x", res);
					break;
				default:
					break;
			}
	return res;
}
int assemble_file(const char *filename) {
	FILE *fp = fopen(filename, "r");
	if (!fp) {
		return EXIT_FAILURE;
	}
	char name[NAME_LEN];
	int32_t res = 0x0;
	const instruction_s *instr;
	char lineBuf[512];
	size_t counter = 0;
	while ((fgets(lineBuf, 512, fp))) {
		sscanf(lineBuf, " %s ", name);
		instr = find_instruction(name, strlen(name));
		if (instr == NULL) {
			log_msg(LOG_ERROR, "[error] unknown instruction:\n%s", name);
			break;
		}
		if( !is_only_ws(lineBuf) ){
		    log_msg(LOG_INFO, "fetching instruction: %s (%c TYPE)", name, instr->type);
			res = get_opcode(instr, lineBuf, name);
			printf("%02lx:\t%08x\t%s", counter, res, lineBuf);
			counter += 4;
		}
	}
	fclose(fp);

	return EXIT_SUCCESS;
}
