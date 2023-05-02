// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * translation_unit.h -- translation_unit ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_TRANSLATION_UNIT_H
#define JKCC_AST_TRANSLATION_UNIT_H


#include <jkcc/ast/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/location.h>
#include <jkcc/vector.h>


typedef struct ast_translation_unit_s {
	ast_t      *external_declaration;
	vector_t    base_type;             // ast_t*
	vector_t    file;                  // file_t*
	ast_t       ast;
} ast_translation_unit_t;


ast_t *ast_translation_unit_append_external_declaration(
	ast_t        *translation_unit,
	ast_t        *external_declaration,
	location_t   *location);
ast_t *ast_translation_unit_init(
	void);
void ast_translation_unit_free(
	ast_t        *ast);
vector_t *ast_translation_unit_get_base_type(
	ast_t        *ast);
vector_t *ast_translation_unit_get_file(
	ast_t        *ast);
void fprint_ast_translation_unit(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_TRANSLATION_UNIT_H */
