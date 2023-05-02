// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * return.h -- return ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_RETURN_H
#define JKCC_AST_RETURN_H


#include <jkcc/ast/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/location.h>


typedef struct ast_return_s {
	ast_t      *expression;
	location_t  location;
	ast_t       ast;
} ast_return_t;


ast_t *ast_return_init(
	ast_t        *expression,
	location_t   *location_start,
	location_t   *location_end);
void ast_return_free(
	ast_t        *ast);
void fprint_ast_return(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_RETURN_H */
