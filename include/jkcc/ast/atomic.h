// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * atomic.h -- atomic ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_ATOMIC_H
#define JKCC_AST_ATOMIC_H


#include <jkcc/ast/ast.h>

#include <stddef.h>
#include <stdio.h>

#include <jkcc/location.h>


typedef struct ast_atomic_s {
	ast_t      *operand;
	location_t  location;
	ast_t       ast;
} ast_atomic_t;


ast_t *ast_atomic_init(
	ast_t         *operand,
	location_t    *location_start,
	location_t    *location_end,
	const char   **error);
void ast_atomic_free(
	ast_t         *ast);
void fprint_ast_atomic(
	FILE          *stream,
	const ast_t   *ast,
	size_t         level,
	uint_fast8_t   flags);


#endif  /* JKCC_AST_ATOMIC_H */
