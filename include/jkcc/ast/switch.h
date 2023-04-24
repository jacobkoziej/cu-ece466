// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * switch.h -- switch ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_SWITCH_H
#define JKCC_AST_SWITCH_H


#include <jkcc/ast/ast.h>

#include <stddef.h>
#include <stdio.h>

#include <jkcc/location.h>


typedef struct ast_switch_s {
	ast_t      *expression;
	ast_t      *statement;
	location_t  location;
	ast_t       ast;
} ast_switch_t;


ast_t *ast_switch_init(
	ast_t        *expression,
	ast_t        *statement,
	location_t   *location_start,
	location_t   *location_end);
void ast_switch_free(
	ast_t        *ast);
void fprint_ast_switch(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_SWITCH_H */
