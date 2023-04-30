// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * array.h -- array ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_ARRAY_H
#define JKCC_AST_ARRAY_H


#include <jkcc/ast/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/location.h>


typedef struct ast_array_s {
	ast_t      *type;
	ast_t      *type_qualifier_list;
	ast_t      *size;
	location_t  location;
	ast_t       ast;
} ast_array_t;


ast_t *ast_array_init(
	ast_t        *type_qualifier_list,
	ast_t        *size,
	location_t   *location_start,
	location_t   *location_end);
void ast_array_free(
	ast_t        *ast);
void ast_array_set_type(
	ast_t        *array,
	ast_t        *type);
void fprint_ast_array(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_ARRAY_H */
