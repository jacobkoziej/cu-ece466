// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * type_name.h -- type_name ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_TYPE_NAME_H
#define JKCC_AST_TYPE_NAME_H


#include <jkcc/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/lexer.h>


typedef struct ast_type_name_s {
	ast_t      *type;                 // ast_t* ast_type_t
	ast_t      *abstract_declarator;  // ast_t* ast_abstract_declarator_t
	location_t  location;
	ast_t       ast;
} ast_type_name_t;


ast_t *ast_type_name_init(
	ast_t        *type,
	ast_t        *abstract_declarator,
	location_t   *location_start,
	location_t   *location_end);
void ast_type_name_free(
	ast_t        *ast);
void fprint_ast_type_name(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_TYPE_NAME_H */
