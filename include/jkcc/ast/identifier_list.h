// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * identifier_list.h -- identifier_list ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_IDENTIFIER_LIST_H
#define JKCC_AST_IDENTIFIER_LIST_H


#include <jkcc/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/location.h>
#include <jkcc/vector.h>


typedef struct ast_identifier_list_s {
	vector_t   identifier;  // ast_t* ast_identifier_t
	location_t location;
	ast_t      ast;
} ast_identifier_list_t;


ast_t *ast_identifier_list_append(
	ast_t        *identifier_list,
	ast_t        *identifier,
	location_t   *location);
ast_t *ast_identifier_list_init(
	ast_t        *identifier,
	location_t   *location);
void ast_identifier_list_free(
	ast_t        *ast);
void fprint_ast_identifier_list(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_IDENTIFIER_LIST_H */
