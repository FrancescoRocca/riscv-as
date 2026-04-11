#ifndef ASSEMBLER_DEBUG_H
#define ASSEMBLER_DEBUG_H

typedef enum log_type {
	LOG_DEBUG,
	LOG_INFO,
	LOG_ERROR,
} log_type_e;

void _log_msg(log_type_e log_type, const char *file, int line, const char *fmt, ...);

#define log_msg(type, fmt, ...) _log_msg(type, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#endif
