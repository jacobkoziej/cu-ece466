// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * unary_operator.h -- unary_operator ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_UNARY_OPERATOR_H
#define JKCC_AST_UNARY_OPERATOR_H


#include <jkcc/ast/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/location.h>


#define AST_UNARY_OPERATOR_AMPERSAND        (1 << 0)
#define AST_UNARY_OPERATOR_ASTERISK         (1 << 1)
#define AST_UNARY_OPERATOR_PLUS             (1 << 2)
#define AST_UNARY_OPERATOR_MINUS            (1 << 3)
#define AST_UNARY_OPERATOR_UNARY_COMPLEMENT (1 << 4)
#define AST_UNARY_OPERATOR_LOGICAL_NOT      (1 << 5)
#define AST_UNARY_OPERATOR_PRE_INCREMENT    (1 << 6)
#define AST_UNARY_OPERATOR_PRE_DECREMENT    (1 << 7)
#define AST_UNARY_OPERATOR_POST_INCREMENT   (1 << 8)
#define AST_UNARY_OPERATOR_POST_DECREMENT   (1 << 9)


typedef struct ast_unary_operator_s {
	ast_t         *operand;
	uint_fast16_t  operator;
	location_t     location;
	ast_t          ast;
} ast_unary_operator_t;


ast_t *ast_unary_operator_init(
	ast_t         *operand,
	uint_fast16_t  operator,
	location_t    *location_start,
	location_t    *location_end);
void ast_unary_operator_free(
	ast_t         *ast);
void fprint_ast_unary_operator(
	FILE          *stream,
	const ast_t   *ast,
	size_t         level,
	uint_fast8_t   flags);


#endif  /* JKCC_AST_UNARY_OPERATOR_H */
