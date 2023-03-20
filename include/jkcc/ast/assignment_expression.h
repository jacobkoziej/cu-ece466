// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * assignment_expression.h -- assignment_expression ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_ASSIGNMENT_EXPRESSION_H
#define JKCC_AST_ASSIGNMENT_EXPRESSION_H


#include <jkcc/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/lexer.h>


typedef struct ast_assignment_expression_s {
	ast_t      *unary_expression;
	ast_t      *assignment_operator;
	ast_t      *assignment_expression;
	location_t  location;
	ast_t       ast;
} ast_assignment_expression_t;


ast_t *ast_assignment_expression_init(
	ast_t        *unary_expression,
	ast_t        *assignment_operator,
	ast_t        *assignment_expression,
	location_t   *location_start,
	location_t   *location_end);
void ast_assignment_expression_free(
	ast_t        *ast);
void fprint_ast_assignment_expression(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_ASSIGNMENT_EXPRESSION_H */
