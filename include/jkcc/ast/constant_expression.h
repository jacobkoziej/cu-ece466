// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * constant_expression.h -- constant_expression ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_CONSTANT_EXPRESSION_H
#define JKCC_AST_CONSTANT_EXPRESSION_H


#include <jkcc/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/lexer.h>


typedef struct ast_constant_expression_s {
	ast_t      *conditional_expression;
	location_t  location;
	ast_t       ast;
} ast_constant_expression_t;


ast_t *ast_constant_expression_init(
	ast_t        *conditional_expression,
	location_t   *location);
void ast_constant_expression_free(
	ast_t        *ast);
void fprint_ast_constant_expression(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_CONSTANT_EXPRESSION_H */
