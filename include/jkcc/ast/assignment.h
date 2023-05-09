// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * assignment.h -- assignment ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_ASSIGNMENT_H
#define JKCC_AST_ASSIGNMENT_H


#include <jkcc/ast/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/location.h>


#define AST_ASSIGNMENT_ASSIGNMENT              (1 << 0)
#define AST_ASSIGNMENT_COMPOUND_MULTIPLICATION (1 << 1)
#define AST_ASSIGNMENT_COMPOUND_DIVISION       (1 << 2)
#define AST_ASSIGNMENT_COMPOUND_MODULO         (1 << 3)
#define AST_ASSIGNMENT_COMPOUND_ADDITION       (1 << 4)
#define AST_ASSIGNMENT_COMPOUND_SUBTRACTION    (1 << 5)
#define AST_ASSIGNMENT_COMPOUND_LBITSHIFT      (1 << 6)
#define AST_ASSIGNMENT_COMPOUND_RBITSHIFT      (1 << 7)
#define AST_ASSIGNMENT_COMPOUND_AND            (1 << 8)
#define AST_ASSIGNMENT_COMPOUND_XOR            (1 << 9)
#define AST_ASSIGNMENT_COMPOUND_OR             (1 << 10)


typedef struct ast_assignment_s {
	ast_t         *lvalue;
	ast_t         *rvalue;
	uint_fast16_t  assignment;
	location_t     location;
	ast_t          ast;
} ast_assignment_t;


ast_t *ast_assignment_init(
	ast_t         *lvalue,
	ast_t         *rvalue,
	uint_fast16_t  assignment,
	location_t    *location_start,
	location_t    *location_end);
void ast_assignment_free(
	ast_t         *ast);
uint_fast16_t ast_assignment_get_assignment(
	ast_t         *ast);
ast_t *ast_assignment_get_lvalue(
	ast_t         *ast);
ast_t *ast_assignment_get_rvalue(
	ast_t         *ast);
void fprint_ast_assignment(
	FILE          *stream,
	const ast_t   *ast,
	size_t         level,
	uint_fast8_t   flags);


#endif  /* JKCC_AST_ASSIGNMENT_H */
