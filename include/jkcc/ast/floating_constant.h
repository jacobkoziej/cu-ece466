// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * floating_constant.h -- floating_constant ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_FLOATING_CONSTANT_H
#define JKCC_AST_FLOATING_CONSTANT_H


#include <jkcc/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/lexer.h>


typedef struct ast_floating_constant_s {
	floating_constant_t floating_constant;
	location_t          location;
	ast_t               ast;
} ast_floating_constant_t;


ast_t *ast_floating_constant_init(
	floating_constant_t *floating_constant,
	location_t          *location);
void ast_floating_constant_free(
	ast_t               *ast);
void fprint_ast_floating_constant(
	FILE                *stream,
	const ast_t         *ast,
	size_t               level,
	uint_fast8_t         flags);


#endif  /* JKCC_AST_FLOATING_CONSTANT_H */
