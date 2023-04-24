// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * alignof.h -- alignof ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_ALIGNOF_H
#define JKCC_AST_ALIGNOF_H


#include <jkcc/ast/ast.h>

#include <stddef.h>
#include <stdio.h>

#include <jkcc/location.h>


typedef struct ast_alignof_s {
	ast_t      *operand;
	location_t  location;
	ast_t       ast;
} ast_alignof_t;


ast_t *ast_alignof_init(
	ast_t        *operand,
	location_t   *location_start,
	location_t   *location_end);
void ast_alignof_free(
	ast_t        *ast);
void fprint_ast_alignof(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_ALIGNOF_H */
