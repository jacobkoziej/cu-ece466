// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * trace.h -- trace program execution
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_TRACE_H
#define JKCC_TRACE_H


#include <stdbool.h>
#include <stdio.h>


#define JKCC_TRACE_LEVEL_NONE   0
#define JKCC_TRACE_LEVEL_LOW    1
#define JKCC_TRACE_LEVEL_MEDIUM 2
#define JKCC_TRACE_LEVEL_HIGH   3

#define TRACE_PRINTF(trace, ...) {    \
	if ((trace)->level)           \
		trace_printf(         \
			trace,        \
			__FILE__,     \
			__func__,     \
			__LINE__,     \
			__VA_ARGS__); \
}

#define TRACE_ARGS(trace, format, ...) { \
	if ((trace)->level)              \
		trace_args(              \
			trace,           \
			__FILE__,        \
			__func__,        \
			format,          \
			#__VA_ARGS__,    \
			__VA_ARGS__);    \
}


typedef struct trace_s {
	FILE *stream;
	bool  ansi_sgr;
	int   level;
} trace_t;


void trace_args(
	trace_t    *trace,
	const char *file,
	const char *func,
	const char *args,
	const char *format,
	...);
void trace_printf(
	trace_t    *trace,
	const char *file,
	const char *func,
	const int   line,
	const char *format,
	...);


#endif  /* JKCC_TRACE_H */
