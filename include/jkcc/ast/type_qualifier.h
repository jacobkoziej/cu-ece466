// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * type_qualifier.h -- type_qualifier ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_TYPE_QUALIFIER_H
#define JKCC_AST_TYPE_QUALIFIER_H


#include <jkcc/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/lexer.h>


typedef struct ast_type_qualifier_s {
	int        type;
	location_t location;
	ast_t      ast;
} ast_type_qualifier_t;


ast_t *ast_type_qualifier_init(
	int           type,
	location_t   *location);
void ast_type_qualifier_free(
	ast_t        *ast);
void fprint_ast_type_qualifier(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_TYPE_QUALIFIER_H */
