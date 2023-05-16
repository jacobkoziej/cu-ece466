// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * while.h -- while ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_WHILE_H
#define JKCC_AST_WHILE_H


#include <jkcc/ast/ast.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/location.h>


typedef struct ast_while_s {
	ast_t      *expression;
	ast_t      *statement;
	bool        do_while;
	location_t  location;
	ast_t       ast;
} ast_while_t;


ast_t *ast_while_init(
	ast_t        *expression,
	ast_t        *statement,
	bool          do_while,
	location_t   *location_start,
	location_t   *location_end);
void ast_while_free(
	ast_t        *ast);
ast_t *ast_while_get_expression(
	ast_t        *ast);
ast_t *ast_while_get_statement(
	ast_t        *ast);
void fprint_ast_while(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_WHILE_H */
