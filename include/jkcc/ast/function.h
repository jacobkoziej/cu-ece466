// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * function.h -- function ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_FUNCTION_H
#define JKCC_AST_FUNCTION_H


#include <jkcc/ast/ast.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/location.h>


typedef struct ast_function_s {
	ast_t        *identifier;
	ast_t        *return_type;
	ast_t        *parameter_list;
	ast_t        *identifier_list;
	ast_t        *declaration_list;
	ast_t        *body;
	bool          variadic;
	location_t    location;
	ast_t         ast;
} ast_function_t;


ast_t *ast_function_init(
	ast_t        *identifier,
	ast_t        *parameter_list,
	ast_t        *identifier_list,
	bool          variadic,
	location_t   *location_start,
	location_t   *location_end);
void ast_function_free(
	ast_t        *ast);
ast_t *ast_function_get_body(
	ast_t        *function);
ast_t *ast_function_get_declaration_list(
	ast_t        *function);
ast_t *ast_function_get_identifier(
	ast_t        *function);
ast_t *ast_function_get_parameter_list(
	ast_t        *function);
ast_t *ast_function_get_return_type(
	ast_t        *function);
void ast_function_set_body(
	ast_t        *function,
	ast_t        *body);
void ast_function_set_declaration_list(
	ast_t        *function,
	ast_t        *declaration_list);
void ast_function_set_return_type(
	ast_t        *function,
	ast_t        *return_type);
void fprint_ast_function(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_FUNCTION_H */
