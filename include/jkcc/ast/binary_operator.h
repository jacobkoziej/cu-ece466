// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * binary_operator.h -- binary_operator ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_BINARY_OPERATOR_H
#define JKCC_AST_BINARY_OPERATOR_H


#include <jkcc/ast/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/location.h>


#define AST_BINARY_OPERATOR_MULTIPLICATION        (1 << 0)
#define AST_BINARY_OPERATOR_DIVISION              (1 << 1)
#define AST_BINARY_OPERATOR_MODULO                (1 << 2)
#define AST_BINARY_OPERATOR_ADDITION              (1 << 3)
#define AST_BINARY_OPERATOR_SUBTRACTION           (1 << 4)
#define AST_BINARY_OPERATOR_LBITSHIFT             (1 << 5)
#define AST_BINARY_OPERATOR_RBITSHIFT             (1 << 6)
#define AST_BINARY_OPERATOR_LESS_THAN             (1 << 7)
#define AST_BINARY_OPERATOR_GREATER_THAN          (1 << 8)
#define AST_BINARY_OPERATOR_LESS_THAN_OR_EQUAL    (1 << 9)
#define AST_BINARY_OPERATOR_GREATER_THAN_OR_EQUAL (1 << 10)
#define AST_BINARY_OPERATOR_EQUALITY              (1 << 11)
#define AST_BINARY_OPERATOR_INEQUALITY            (1 << 12)
#define AST_BINARY_OPERATOR_AND                   (1 << 13)
#define AST_BINARY_OPERATOR_EXCLUSIVE_OR          (1 << 14)
#define AST_BINARY_OPERATOR_INCLUSIVE_OR          (1 << 15)
#define AST_BINARY_OPERATOR_LOGICAL_AND           (1 << 16)
#define AST_BINARY_OPERATOR_LOGICAL_OR            (1 << 17)


typedef struct ast_binary_operator_s {
	ast_t         *lhs;
	ast_t         *rhs;
	uint_fast32_t  operator;
	location_t     location;
	ast_t          ast;
} ast_binary_operator_t;


ast_t *ast_binary_operator_init(
	ast_t         *lhs,
	ast_t         *rhs,
	uint_fast32_t  operator,
	location_t    *location_start,
	location_t    *location_end);
void ast_binary_operator_free(
	ast_t         *ast);
ast_t *ast_binary_operator_get_lhs(
	ast_t         *ast);
uint_fast32_t ast_binary_operator_get_operator(
	ast_t         *ast);
void fprint_ast_binary_operator(
	FILE          *stream,
	const ast_t   *ast,
	size_t         level,
	uint_fast8_t   flags);


#endif  /* JKCC_AST_BINARY_OPERATOR_H */
