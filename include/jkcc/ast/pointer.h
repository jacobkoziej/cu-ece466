// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * pointer.h -- pointer ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_POINTER_H
#define JKCC_AST_POINTER_H


#include <jkcc/ast/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/location.h>


typedef struct ast_pointer_s {
	ast_t      *pointer;              // ast_t* ast_pointer_t
	ast_t      *type_qualifier_list;  // ast_t* ast_type_qualifier_list_t
	location_t  location;
	ast_t       ast;
} ast_pointer_t;


void ast_pointer_append(
	ast_t        *pointer,
	ast_t        *type);
ast_t *ast_pointer_init(
	ast_t        *pointer,
	ast_t        *type_qualifier_list,
	location_t   *location);
void ast_pointer_free(
	ast_t        *ast);
void fprint_ast_pointer(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_POINTER_H */
