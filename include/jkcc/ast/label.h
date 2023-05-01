// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * label.h -- label ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_LABEL_H
#define JKCC_AST_LABEL_H


#include <jkcc/ast/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/location.h>


typedef struct ast_label_s {
	ast_t      *identifier;
	ast_t      *statement;
	location_t  location;
	ast_t       ast;
} ast_label_t;


ast_t *ast_label_init(
	ast_t        *identifier,
	ast_t        *statement,
	location_t   *location_start,
	location_t   *location_end);
void ast_label_free(
	ast_t        *ast);
void fprint_ast_label(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_LABEL_H */
