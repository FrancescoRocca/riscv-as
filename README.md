# Assembler

Small assembler for our RISC-V core.

## Building

```bash
$ meson setup build
$ cd build
$ meson compile
```

### Definitions schema

Where you find `0xFF` it means that it doesn't have a `func7` field.

### GPERF

```
cd gperf && gperf -t -C test.gperf --output-file=hash_utils.c
```

Add the hash header file in hash_utils.c:

```
#include "hash.h"
```

