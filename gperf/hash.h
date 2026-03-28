#ifndef ASSEMBLER_HASH_H
#define ASSEMBLER_HASH_H

#include <stddef.h>

const struct instruction *find_instruction(register const char *str, register size_t len);

#endif
