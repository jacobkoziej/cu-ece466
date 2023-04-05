// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * type.h -- type ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_TYPE_H
#define JKCC_AST_TYPE_H


#include <jkcc/ast.h>
#include <jkcc/ast/type_specifier.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/location.h>
#include <jkcc/vector.h>


typedef struct ast_type_s {
	vector_t       storage_class_specifier_vector;  // ast_t* ast_storage_class_specifier_t
	vector_t       type_specifier_vector;           // ast_t* ast_type_specifier_t
	vector_t       type_qualifier_vector;           // ast_t* ast_type_qualifier_t
	vector_t       function_specifier_vector;       // ast_t* ast_function_specifier_t
	vector_t       alignment_specifier_vector;      // ast_t* ast_alignas_t
	uint_fast8_t   storage_class_specifier;
	uint_fast16_t  type_specifier;
	uint_fast8_t   type_qualifier;
	uint_fast8_t   function_specifier;
	location_t     location;
	ast_t          ast;
} ast_type_t;


ast_t *ast_type_append(
	ast_t         *type,
	ast_t         *specifier,
	location_t    *location,
	const char   **error);
ast_t *ast_type_init(
	ast_t         *specifier,
	location_t    *location);
void ast_type_free(
	ast_t         *ast);
void fprint_ast_type(
	FILE          *stream,
	const ast_t   *ast,
	size_t         level,
	uint_fast8_t   flags);


#endif  /* JKCC_AST_TYPE_H */
