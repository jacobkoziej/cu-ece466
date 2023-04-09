// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * parser.h -- rudimentary parser
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_PARSER_H
#define JKCC_PARSER_H


#include <jkcc/ast.h>
#include <jkcc/lexer.h>
#include <jkcc/symbol.h>
#include <jkcc/trace.h>
#include <jkcc/vector.h>


typedef struct parser_s {
	const char *path;
	trace_t    *trace;
	yyextra_t  *yyextra_data;
} parser_t;

typedef struct translation_unit_s {
	ast_t    *ast;
	vector_t  file;               // file_t*
	scope_t  *symbol_table;
} translation_unit_t;


translation_unit_t *parse(
	parser_t           *parser);
void translation_unit_free(
	translation_unit_t *translation_unit);


#endif  /* JKCC_PARSER_H */
