// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * if.h -- if ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_IF_H
#define JKCC_AST_IF_H


#include <jkcc/ast/ast.h>

#include <stddef.h>
#include <stdio.h>

#include <jkcc/location.h>


typedef struct ast_if_s {
	ast_t      *expression;
	ast_t      *true_statement;
	ast_t      *false_statement;
	location_t  location;
	ast_t       ast;
} ast_if_t;


ast_t *ast_if_init(
	ast_t        *expression,
	ast_t        *true_statement,
	ast_t        *false_statement,
	location_t   *location_start,
	location_t   *location_end);
void ast_if_free(
	ast_t        *ast);
ast_t *ast_if_get_false_statement(
	ast_t        *ast);
ast_t *ast_if_get_true_statement(
	ast_t        *ast);
void fprint_ast_if(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_IF_H */
