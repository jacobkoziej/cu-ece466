// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * for.h -- for ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_FOR_H
#define JKCC_AST_FOR_H


#include <jkcc/ast/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/location.h>


typedef struct ast_for_s {
	ast_t      *initializer;
	ast_t      *condition;
	ast_t      *iteration;
	ast_t      *statement;
	location_t  location;
	ast_t       ast;
} ast_for_t;


ast_t *ast_for_init(
	ast_t        *initializer,
	ast_t        *condition,
	ast_t        *iteration,
	ast_t        *statement,
	location_t   *location_start,
	location_t   *location_end);
void ast_for_free(
	ast_t        *ast);
void fprint_ast_for(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_FOR_H */
