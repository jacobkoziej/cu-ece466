// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * list.h -- list ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_LIST_H
#define JKCC_AST_LIST_H


#include <jkcc/ast/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/location.h>
#include <jkcc/vector.h>


typedef struct ast_list_s {
	vector_t   list;
	location_t location;
	ast_t      ast;
} ast_list_t;


ast_t *ast_list_append(
	ast_t        *list,
	ast_t        *ast,
	location_t   *location);
ast_t *ast_list_init(
	ast_t        *ast,
	location_t   *location);
void ast_list_free(
	ast_t        *ast);
vector_t *ast_list_get_list(
	ast_t        *ast);
void fprint_ast_list(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_LIST_H */
