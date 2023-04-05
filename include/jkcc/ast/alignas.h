// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * alignas.h -- alignas ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_ALIGNAS_H
#define JKCC_AST_ALIGNAS_H


#include <jkcc/ast.h>

#include <stddef.h>
#include <stdio.h>

#include <jkcc/location.h>


typedef struct ast_alignas_s {
	ast_t      *operand;
	location_t  location;
	ast_t       ast;
} ast_alignas_t;


ast_t *ast_alignas_init(
	ast_t        *operand,
	location_t   *location_start,
	location_t   *location_end);
void ast_alignas_free(
	ast_t        *ast);
void fprint_ast_alignas(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_ALIGNAS_H */
