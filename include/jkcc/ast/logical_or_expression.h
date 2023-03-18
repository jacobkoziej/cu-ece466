// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * logical_or_expression.h -- logical_or_expression ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_LOGICAL_OR_EXPRESSION_H
#define JKCC_AST_LOGICAL_OR_EXPRESSION_H


#include <jkcc/ast.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/lexer.h>


typedef struct ast_logical_or_expression_s {
	ast_t      *logical_or_expression;
	ast_t      *logical_and_expression;
	bool        logical_or;
	location_t  location;
	ast_t       ast;
} ast_logical_or_expression_t;


ast_t *ast_logical_or_expression_init(
	ast_t        *logical_or_expression,
	ast_t        *logical_and_expression,
	bool          logical_or,
	location_t   *location_start,
	location_t   *location_end);
void ast_logical_or_expression_free(
	ast_t        *ast);
void fprint_ast_logical_or_expression(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_LOGICAL_OR_EXPRESSION_H */
