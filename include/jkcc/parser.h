// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * parser.h -- rudimentary parser
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_PARSER_H
#define JKCC_PARSER_H


#include <sys/types.h>


typedef struct location_s {
	struct {
		off_t offset;
		int   line;
		int   column;
	} start;
	struct {
		off_t offset;
		int   line;
		int   column;
	} end;
} location_t;


#endif  /* JKCC_PARSER_H */
