// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * init_declarator_list.h -- init_declarator_list ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_INIT_DECLARATOR_LIST_H
#define JKCC_AST_INIT_DECLARATOR_LIST_H


#include <jkcc/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/location.h>
#include <jkcc/vector.h>


typedef struct ast_init_declarator_list_s {
	vector_t   init_declarator;
	location_t location;
	ast_t      ast;
} ast_init_declarator_list_t;


ast_t *ast_init_declarator_list_append(
	ast_t        *init_declarator_list,
	ast_t        *init_declarator,
	location_t   *location);
ast_t *ast_init_declarator_list_init(
	ast_t        *init_declarator_list,
	ast_t        *init_declarator,
	location_t   *location_start,
	location_t   *location_end);
void ast_init_declarator_list_free(
	ast_t        *ast);
void fprint_ast_init_declarator_list(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_INIT_DECLARATOR_LIST_H */
