// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * declaration_list.h -- declaration_list ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_DECLARATION_LIST_H
#define JKCC_AST_DECLARATION_LIST_H


#include <jkcc/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/location.h>
#include <jkcc/vector.h>


typedef struct ast_declaration_list_s {
	vector_t   declaration;
	location_t location;
	ast_t      ast;
} ast_declaration_list_t;


ast_t *ast_declaration_list_append(
	ast_t        *declaration_list,
	ast_t        *declaration,
	location_t   *location);
ast_t *ast_declaration_list_init(
	ast_t        *declaration_list,
	ast_t        *declaration,
	location_t   *location_start,
	location_t   *location_end);
void ast_declaration_list_free(
	ast_t        *ast);
void fprint_ast_declaration_list(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_DECLARATION_LIST_H */
