// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * relational_expression.h -- relational_expression ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_RELATIONAL_EXPRESSION_H
#define JKCC_AST_RELATIONAL_EXPRESSION_H


#include <jkcc/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/lexer.h>


#define RELATIONAL_EXPRESSION_LESS_THAN             (1 << 0)
#define RELATIONAL_EXPRESSION_GREATER_THAN          (1 << 1)
#define RELATIONAL_EXPRESSION_LESS_THAN_OR_EQUAL    (1 << 2)
#define RELATIONAL_EXPRESSION_GREATER_THAN_OR_EQUAL (1 << 3)


typedef struct ast_relational_expression_s {
	ast_t        *relational_expression;
	ast_t        *shift_expression;
	uint_fast8_t  relation;
	location_t    location;
	ast_t         ast;
} ast_relational_expression_t;


ast_t *ast_relational_expression_init(
	ast_t        *relational_expression,
	ast_t        *shift_expression,
	uint_fast8_t  relation,
	location_t   *location_start,
	location_t   *location_end);
void ast_relational_expression_free(
	ast_t        *ast);
void fprint_ast_relational_expression(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_RELATIONAL_EXPRESSION_H */
