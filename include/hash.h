#ifndef ASSEMBLER_HASH_H
#define ASSEMBLER_HASH_H

#include <stdint.h>

/* Way less but we avoid collisions. */
#define OPCODES_NUM 512

unsigned long sdbm(char *keyword);

typedef struct hashmap {
	const uint8_t opcodes[OPCODES_NUM];
} hashmap_s;

#endif
