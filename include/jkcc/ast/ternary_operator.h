// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * ternary_operator.h -- ternary_operator ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_TERNARY_OPERATOR_H
#define JKCC_AST_TERNARY_OPERATOR_H


#include <jkcc/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/lexer.h>


typedef struct ast_ternary_operator_s {
	ast_t      *condition;
	ast_t      *lhs;
	ast_t      *rhs;
	location_t  location;
	ast_t       ast;
} ast_ternary_operator_t;


ast_t *ast_ternary_operator_init(
	ast_t        *condition,
	ast_t        *lhs,
	ast_t        *rhs,
	location_t   *location_start,
	location_t   *location_end);
void ast_ternary_operator_free(
	ast_t        *ast);
void fprint_ast_ternary_operator(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_TERNARY_OPERATOR_H */
