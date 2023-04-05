// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * sizeof.h -- sizeof ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_SIZEOF_H
#define JKCC_AST_SIZEOF_H


#include <jkcc/ast.h>

#include <stddef.h>
#include <stdio.h>

#include <jkcc/location.h>


typedef struct ast_sizeof_s {
	ast_t      *operand;
	location_t  location;
	ast_t       ast;
} ast_sizeof_t;


ast_t *ast_sizeof_init(
	ast_t        *operand,
	location_t   *location_start,
	location_t   *location_end);
void ast_sizeof_free(
	ast_t        *ast);
void fprint_ast_sizeof(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_SIZEOF_H */
