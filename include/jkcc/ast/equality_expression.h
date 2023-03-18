// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * equality_expression.h -- equality_expression ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_EQUALITY_EXPRESSION_H
#define JKCC_AST_EQUALITY_EXPRESSION_H


#include <jkcc/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/lexer.h>


#define EQUALITY_EXPRESSION_EQUALITY   (1 << 0)
#define EQUALITY_EXPRESSION_INEQUALITY (1 << 1)


typedef struct ast_equality_expression_s {
	ast_t        *equality_expression;
	ast_t        *relational_expression;
	uint_fast8_t  equality;
	location_t    location;
	ast_t         ast;
} ast_equality_expression_t;


ast_t *ast_equality_expression_init(
	ast_t        *equality_expression,
	ast_t        *relational_expression,
	uint_fast8_t  equality,
	location_t   *location_start,
	location_t   *location_end);
void ast_equality_expression_free(
	ast_t        *ast);
void fprint_ast_equality_expression(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_EQUALITY_EXPRESSION_H */
