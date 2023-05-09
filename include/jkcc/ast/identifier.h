// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * identifier.h -- identifier ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_IDENTIFIER_H
#define JKCC_AST_IDENTIFIER_H


#include <jkcc/ast/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/constant.h>
#include <jkcc/location.h>
#include <jkcc/string.h>


typedef struct ast_identifier_s {
	identifier_t  identifier;
	ast_t        *type;
	location_t    location;
	ast_t         ast;
} ast_identifier_t;


ast_t *ast_identifier_init(
	identifier_t *identifier,
	location_t   *location);
void ast_identifier_free(
	ast_t        *ast);
const string_t *ast_identifier_get_string(
	ast_t        *identifier);
ast_t *ast_identifier_get_type(
	ast_t        *identifier);
void ast_identifier_set_type(
	ast_t        *identifier,
	ast_t        *type);
void fprint_ast_identifier(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_IDENTIFIER_H */
