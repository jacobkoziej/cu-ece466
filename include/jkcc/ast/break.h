// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * break.h -- break ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_BREAK_H
#define JKCC_AST_BREAK_H


#include <jkcc/ast/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/location.h>


typedef struct ast_break_s {
	location_t location;
	ast_t      ast;
} ast_break_t;


ast_t *ast_break_init(
	location_t   *location);
void ast_break_free(
	ast_t        *ast);
void fprint_ast_break(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_BREAK_H */
