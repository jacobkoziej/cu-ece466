// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * continue.h -- continue ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_CONTINUE_H
#define JKCC_AST_CONTINUE_H


#include <jkcc/ast/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/location.h>


typedef struct ast_continue_s {
	location_t location;
	ast_t      ast;
} ast_continue_t;


ast_t *ast_continue_init(
	location_t   *location);
void ast_continue_free(
	ast_t        *ast);
void fprint_ast_continue(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_CONTINUE_H */
