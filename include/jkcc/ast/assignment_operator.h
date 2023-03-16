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


typedef struct ast_assignment_operator_s {
	int        type;
	location_t location;
	ast_t      ast;
} ast_assignment_operator_t;


ast_t *ast_assignment_operator_init(
	int           type,
	location_t   *location);
void ast_assignment_operator_free(
	ast_t        *ast);
void fprint_ast_assignment_operator(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_ASSIGNMENT_OPERATOR_H */
