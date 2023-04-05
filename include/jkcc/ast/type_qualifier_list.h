// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * type_qualifier_list.h -- type_qualifier_list ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_TYPE_QUALIFIER_LIST_H
#define JKCC_AST_TYPE_QUALIFIER_LIST_H


#include <jkcc/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/location.h>
#include <jkcc/vector.h>


typedef struct ast_type_qualifier_list_s {
	vector_t   type_qualifier;  // ast_t* ast_type_qualifier_t
	location_t location;
	ast_t      ast;
} ast_type_qualifier_list_t;


ast_t *ast_type_qualifier_list_append(
	ast_t        *type_qualifier_list,
	ast_t        *type_qualifier,
	location_t   *location);
ast_t *ast_type_qualifier_list_init(
	ast_t        *type_qualifier,
	location_t   *location);
void ast_type_qualifier_list_free(
	ast_t        *ast);
void fprint_ast_type_qualifier_list(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_TYPE_QUALIFIER_LIST_H */
