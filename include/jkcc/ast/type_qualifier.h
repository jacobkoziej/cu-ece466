// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * type_qualifier.h -- type_qualifier ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_TYPE_QUALIFIER_H
#define JKCC_AST_TYPE_QUALIFIER_H


#include <jkcc/ast/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/location.h>


#define AST_TYPE_QUALIFIER_CONST    (1 << 0)
#define AST_TYPE_QUALIFIER_RESTRICT (1 << 1)
#define AST_TYPE_QUALIFIER_VOLATILE (1 << 2)
#define AST_TYPE_QUALIFIER__ATOMIC  (1 << 3)


typedef struct ast_type_qualifier_s {
	uint_fast8_t qualifier;
	location_t   location;
	ast_t        ast;
} ast_type_qualifier_t;


ast_t *ast_type_qualifier_init(
	uint_fast8_t  qualifier,
	location_t   *location);
void ast_type_qualifier_free(
	ast_t        *ast);
void fprint_ast_type_qualifier(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_TYPE_QUALIFIER_H */
