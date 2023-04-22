// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * generic_association_list.h -- generic_association_list ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_GENERIC_ASSOCIATION_LIST_H
#define JKCC_AST_GENERIC_ASSOCIATION_LIST_H


#include <jkcc/ast/ast.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include <jkcc/location.h>
#include <jkcc/vector.h>


typedef struct ast_generic_association_list_s {
	vector_t   generic_association;  // ast_t* ast_generic_association_t
	bool       default_association;
	location_t location;
	ast_t      ast;
} ast_generic_association_list_t;


ast_t *ast_generic_association_list_append(
	ast_t         *generic_association_list,
	ast_t         *generic_association,
	location_t    *location,
	const char   **error);
ast_t *ast_generic_association_list_init(
	ast_t         *generic_association,
	location_t    *location);
void ast_generic_association_list_free(
	ast_t         *ast);
void fprint_ast_generic_association_list(
	FILE          *stream,
	const ast_t   *ast,
	size_t         level,
	uint_fast8_t   flags);


#endif  /* JKCC_AST_GENERIC_ASSOCIATION_LIST_H */
