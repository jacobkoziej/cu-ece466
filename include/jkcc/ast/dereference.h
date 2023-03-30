// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * dereference.h -- dereference ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_DEREFERENCE_H
#define JKCC_AST_DEREFERENCE_H


#include <jkcc/ast.h>

#include <stddef.h>
#include <stdio.h>

#include <jkcc/lexer.h>


typedef struct ast_dereference_s {
	ast_t      *operand;
	location_t  location;
	ast_t       ast;
} ast_dereference_t;


ast_t *ast_dereference_init(
	ast_t        *operand,
	location_t   *location_start,
	location_t   *location_end);
void ast_dereference_free(
	ast_t        *ast);
void fprint_ast_dereference(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_DEREFERENCE_H */
