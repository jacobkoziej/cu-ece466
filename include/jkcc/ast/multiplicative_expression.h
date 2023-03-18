// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * multiplicative_expression.h -- multiplicative_expression ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_MULTIPLICATIVE_EXPRESSION_H
#define JKCC_AST_MULTIPLICATIVE_EXPRESSION_H


#include <jkcc/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/lexer.h>


#define MULTIPLICATIVE_EXPRESSION_MULTIPLICATION (1 << 0)
#define MULTIPLICATIVE_EXPRESSION_DIVISION       (1 << 1)
#define MULTIPLICATIVE_EXPRESSION_MODULO         (1 << 2)


typedef struct ast_multiplicative_expression_s {
	ast_t        *multiplicative_expression;
	ast_t        *cast_expression;
	uint_fast8_t  operation;
	location_t    location;
	ast_t         ast;
} ast_multiplicative_expression_t;


ast_t *ast_multiplicative_expression_init(
	ast_t        *multiplicative_expression,
	ast_t        *cast_expression,
	uint_fast8_t  operation,
	location_t   *location_start,
	location_t   *location_end);
void ast_multiplicative_expression_free(
	ast_t        *ast);
void fprint_ast_multiplicative_expression(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_MULTIPLICATIVE_EXPRESSION_H */
