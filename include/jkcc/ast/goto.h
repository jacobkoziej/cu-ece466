// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * goto.h -- goto ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_GOTO_H
#define JKCC_AST_GOTO_H


#include <jkcc/ast/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/location.h>


typedef struct ast_goto_s {
	ast_t      *identifier;
	ast_t      *label;
	location_t  location;
	ast_t       ast;
} ast_goto_t;


ast_t *ast_goto_init(
	ast_t        *identifier,
	location_t   *location_start,
	location_t   *location_end);
void ast_goto_free(
	ast_t        *ast);
ast_t *ast_goto_get_identifier(
	ast_t        *ast_goto);
void ast_goto_set_label(
	ast_t        *ast_goto,
	ast_t        *label);
void fprint_ast_goto(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_GOTO_H */
