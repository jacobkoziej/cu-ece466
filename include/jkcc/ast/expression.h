// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * expression.h -- expression ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_EXPRESSION_H
#define JKCC_AST_EXPRESSION_H


#include <jkcc/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/lexer.h>
#include <jkcc/vector.h>


typedef struct ast_expression_s {
	vector_t   assignment_expression;
	location_t location;
	ast_t      ast;
} ast_expression_t;


ast_t *ast_expression_append(
	ast_t        *expression,
	ast_t        *assignment_expression,
	location_t   *location);
ast_t *ast_expression_init(
	ast_t        *assignment_expression,
	location_t   *location);
void ast_expression_free(
	ast_t        *ast);
void fprint_ast_expression(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_EXPRESSION_H */
