// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * static_assert.h -- static_assert ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_STATIC_ASSERT_H
#define JKCC_AST_STATIC_ASSERT_H


#include <jkcc/ast.h>

#include <stddef.h>
#include <stdio.h>

#include <jkcc/lexer.h>


typedef struct ast_static_assert_s {
	ast_t      *constant_expression;
	ast_t      *string_literal;
	location_t  location;
	ast_t       ast;
} ast_static_assert_t;


ast_t *ast_static_assert_init(
	ast_t        *constant_expression,
	ast_t        *string_literal,
	location_t   *location_start,
	location_t   *location_end);
void ast_static_assert_free(
	ast_t        *ast);
void fprint_ast_static_assert(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_STATIC_ASSERT_H */
