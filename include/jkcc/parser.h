// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * parser.h -- rudimentary parser
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_PARSER_H
#define JKCC_PARSER_H


#include <sys/types.h>


typedef struct location_s {
	off_t start_offset;
	int   start_line;
	int   start_column;
	off_t end_offset;
	int   end_line;
	int   end_column;
} location_t;


#endif  /* JKCC_PARSER_H */
