// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * unary_operator.h -- unary_operator ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_UNARY_OPERATOR_H
#define JKCC_AST_UNARY_OPERATOR_H


#include <jkcc/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/lexer.h>


#define UNARY_OPERATOR_AMPERSAND        (1 << 0)
#define UNARY_OPERATOR_ASTERISK         (1 << 1)
#define UNARY_OPERATOR_PLUS             (1 << 2)
#define UNARY_OPERATOR_MINUS            (1 << 3)
#define UNARY_OPERATOR_UNARY_COMPLEMENT (1 << 4)
#define UNARY_OPERATOR_LOGICAL_NOT      (1 << 5)


typedef struct ast_unary_operator_s {
	uint_fast8_t operator;
	location_t   location;
	ast_t        ast;
} ast_unary_operator_t;


ast_t *ast_unary_operator_init(
	uint_fast8_t  operator,
	location_t   *location);
void ast_unary_operator_free(
	ast_t        *ast);
void fprint_ast_unary_operator(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_UNARY_OPERATOR_H */
