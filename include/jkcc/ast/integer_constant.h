// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * integer_constant.h -- integer_constant ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_INTEGER_CONSTANT_H
#define JKCC_AST_INTEGER_CONSTANT_H


#include <jkcc/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/lexer.h>


typedef struct ast_integer_constant_s {
	integer_constant_t integer_constant;
	location_t         location;
	ast_t              ast;
} ast_integer_constant_t;


ast_t *ast_integer_constant_init(
	integer_constant_t *integer_constant,
	location_t         *location);
void ast_integer_constant_free(
	ast_t              *ast);
void fprint_ast_integer_consant(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_INTEGER_CONSTANT_H */
