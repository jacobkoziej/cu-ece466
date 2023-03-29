// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * cast.h -- cast ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_CAST_H
#define JKCC_AST_CAST_H


#include <jkcc/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/lexer.h>


typedef struct ast_cast_s {
	ast_t      *expression;
	ast_t      *type;
	location_t  location;
	ast_t       ast;
} ast_cast_t;


ast_t *ast_cast_init(
	ast_t        *expression,
	ast_t        *type,
	location_t   *location_start,
	location_t   *location_end);
void ast_cast_free(
	ast_t        *ast);
void fprint_ast_cast(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_CAST_H */
