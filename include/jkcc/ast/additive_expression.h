// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * additive_expression.h -- additive_expression ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_ADDITIVE_EXPRESSION_H
#define JKCC_AST_ADDITIVE_EXPRESSION_H


#include <jkcc/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/lexer.h>


#define ADDITIVE_EXPRESSION_ADDITION    (1 << 0)
#define ADDITIVE_EXPRESSION_SUBTRACTION (1 << 1)


typedef struct ast_additive_expression_s {
	ast_t        *additive_expression;
	ast_t        *multiplicative_expression;
	uint_fast8_t  operation;
	location_t    location;
	ast_t         ast;
} ast_additive_expression_t;


ast_t *ast_additive_expression_init(
	ast_t        *additive_expression,
	ast_t        *multiplicative_expression,
	uint_fast8_t  operation,
	location_t   *location_start,
	location_t   *location_end);
void ast_additive_expression_free(
	ast_t        *ast);
void fprint_ast_additive_expression(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_ADDITIVE_EXPRESSION_H */
