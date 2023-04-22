// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * argument_list.h -- argument_list ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_ARGUMENT_LIST_H
#define JKCC_AST_ARGUMENT_LIST_H


#include <jkcc/ast/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/location.h>
#include <jkcc/vector.h>


typedef struct ast_argument_list_s {
	vector_t   argument;
	location_t location;
	ast_t      ast;
} ast_argument_list_t;


ast_t *ast_argument_list_append(
	ast_t        *argument_list,
	ast_t        *argument,
	location_t   *location);
ast_t *ast_argument_list_init(
	ast_t        *argument,
	location_t   *location);
void ast_argument_list_free(
	ast_t        *ast);
void fprint_ast_argument_list(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_ARGUMENT_LIST_H */
