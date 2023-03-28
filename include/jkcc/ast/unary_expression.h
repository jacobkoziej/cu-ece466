// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * unary_expression.h -- unary_expression ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_UNARY_EXPRESSION_H
#define JKCC_AST_UNARY_EXPRESSION_H


#include <jkcc/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/lexer.h>


#define UNARY_EXPRESSION_INCREMENT (1 << 0)
#define UNARY_EXPRESSION_DECREMENT (1 << 1)


typedef struct ast_unary_expression_s {
	ast_t        *unary_expression;
	ast_t        *unary_operator;
	ast_t        *cast_expression;
	uint_fast8_t  flags;
	location_t    location;
	ast_t         ast;
} ast_unary_expression_t;


ast_t *ast_unary_expression_init(
	ast_t        *unary_expression,
	ast_t        *unary_operator,
	ast_t        *cast_expression,
	uint_fast8_t  flags,
	location_t   *location_start,
	location_t   *location_end);
void ast_unary_expression_free(
	ast_t        *ast);
void fprint_ast_unary_expression(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_UNARY_EXPRESSION_H */
