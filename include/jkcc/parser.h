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
	vector_t  file;  // file_t*
} parse_t;


parse_t *parse(const char *path);
void     parse_free(parse_t *translation_unit);


#endif  /* JKCC_PARSER_H */
