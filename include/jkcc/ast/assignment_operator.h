// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * assignment_operator.h -- assignment_operator ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_ASSIGNMENT_OPERATOR_H
#define JKCC_AST_ASSIGNMENT_OPERATOR_H


#include <jkcc/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/lexer.h>


#define ASSIGNMENT_OPERATOR_ASSIGNMENT              (1 << 0)
#define ASSIGNMENT_OPERATOR_COMPOUND_MULTIPLICATION (1 << 1)
#define ASSIGNMENT_OPERATOR_COMPOUND_DIVISION       (1 << 2)
#define ASSIGNMENT_OPERATOR_COMPOUND_MODULO         (1 << 3)
#define ASSIGNMENT_OPERATOR_COMPOUND_ADDITION       (1 << 4)
#define ASSIGNMENT_OPERATOR_COMPOUND_SUBTRACTION    (1 << 5)
#define ASSIGNMENT_OPERATOR_COMPOUND_LBITSHIFT      (1 << 6)
#define ASSIGNMENT_OPERATOR_COMPOUND_RBITSHIFT      (1 << 7)
#define ASSIGNMENT_OPERATOR_COMPOUND_AND            (1 << 8)
#define ASSIGNMENT_OPERATOR_COMPOUND_XOR            (1 << 9)
#define ASSIGNMENT_OPERATOR_COMPOUND_OR             (1 << 10)

typedef struct ast_assignment_operator_s {
	uint_fast16_t operator;
	location_t    location;
	ast_t         ast;
} ast_assignment_operator_t;


ast_t *ast_assignment_operator_init(
	uint_fast16_t  operator,
	location_t    *location);
void ast_assignment_operator_free(
	ast_t         *ast);
void fprint_ast_assignment_operator(
	FILE          *stream,
	const ast_t   *ast,
	size_t         level,
	uint_fast8_t   flags);


#endif  /* JKCC_AST_ASSIGNMENT_OPERATOR_H */
