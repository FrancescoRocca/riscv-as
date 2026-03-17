#ifndef ASSEMBLER_HASH_H
#define ASSEMBLER_HASH_H

#include <stdint.h>

#define OPCODES_NUM 64 // Way less but we keep some space for the future.

unsigned long sdbm(char *keyword);

// uint8_t opcodes[OPCODES_NUM];

#endif
