// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * trace.h -- trace program execution
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_TRACE_H
#define JKCC_TRACE_H


#include <stdbool.h>
#include <stdio.h>


typedef struct trace_s {
	FILE *stream;
	bool  ansi_sgr;
	int   level;
} trace_t;


#endif  /* JKCC_TRACE_H */
