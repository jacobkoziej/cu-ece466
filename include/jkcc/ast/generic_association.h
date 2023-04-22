// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * generic_association.h -- generic_association ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_GENERIC_ASSOCIATION_H
#define JKCC_AST_GENERIC_ASSOCIATION_H


#include <jkcc/ast/ast.h>

#include <stddef.h>
#include <stdio.h>

#include <jkcc/location.h>


typedef struct ast_generic_association_s {
	ast_t      *type;
	ast_t      *expression;
	location_t  location;
	ast_t       ast;
} ast_generic_association_t;


ast_t *ast_generic_association_init(
	ast_t        *type,
	ast_t        *expression,
	location_t   *location_start,
	location_t   *location_end);
void ast_generic_association_free(
	ast_t        *ast);
void fprint_ast_generic_association(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_GENERIC_ASSOCIATION_H */
