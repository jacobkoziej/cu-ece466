// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * string_literal.h -- string_literal ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_STRING_LITERAL_H
#define JKCC_AST_STRING_LITERAL_H


#include <jkcc/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/constant.h>
#include <jkcc/location.h>


typedef struct ast_string_literal_s {
	string_literal_t string_literal;
	location_t       location;
	ast_t            ast;
} ast_string_literal_t;


ast_t *ast_string_literal_init(
	string_literal_t *string_literal,
	location_t       *location);
void ast_string_literal_free(
	ast_t            *ast);
void fprint_ast_string_literal(
	FILE             *stream,
	const ast_t      *ast,
	size_t            level,
	uint_fast8_t      flags);


#endif  /* JKCC_AST_STRING_LITERAL_H */
