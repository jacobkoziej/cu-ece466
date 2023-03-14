// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * parser.h -- rudimentary parser
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_PARSER_H
#define JKCC_PARSER_H


#include <jkcc/ast.h>
#include <jkcc/vector.h>


typedef struct parse_s {
	ast_t    *ast;
	vector_t  file;
} parse_t;


#endif  /* JKCC_PARSER_H */
