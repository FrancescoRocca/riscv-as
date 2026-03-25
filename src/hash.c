#include "hash.h"

/* Simple hash sdbm function implementation */
/* Implementation from gawk, not our copyright. */
unsigned long sdbm(char *keyword) {
	unsigned long hash = 0;
	int c;

	while ((c = *keyword++)) {
		hash = c + (hash << 6) + (hash << 16) - hash;
	}

	return hash;
}
