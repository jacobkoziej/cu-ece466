// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * generic_selection.h -- generic_selection ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_GENERIC_SELECTION_H
#define JKCC_AST_GENERIC_SELECTION_H


#include <jkcc/ast/ast.h>

#include <stddef.h>
#include <stdio.h>

#include <jkcc/location.h>


typedef struct ast_generic_selection_s {
	ast_t      *expression;
	ast_t      *generic_association_list;
	location_t  location;
	ast_t       ast;
} ast_generic_selection_t;


ast_t *ast_generic_selection_init(
	ast_t        *expression,
	ast_t        *generic_association_list,
	location_t   *location_start,
	location_t   *location_end);
void ast_generic_selection_free(
	ast_t        *ast);
void fprint_ast_generic_selection(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_GENERIC_SELECTION_H */
