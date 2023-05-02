// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * translation_unit.c -- translation_unit ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast/ast.h>
#include <jkcc/ast/translation_unit.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/location.h>
#include <jkcc/vector.h>


ast_t *ast_translation_unit_append_external_declaration(
	ast_t      *ast,
	ast_t      *external_declaration,
	location_t *location)
{
	ast_translation_unit_t *node = OFFSETOF_AST_NODE(
		ast,
		ast_translation_unit_t);

	ast_t *list = (!node->external_declaration)
		? ast_list_init(external_declaration, location)
		: ast_list_append(
			node->external_declaration,
			external_declaration,
			location);

	if (!list) return NULL;

	node->external_declaration = list;

	return ast;
}

ast_t *ast_translation_unit_init(
	void)
{
	AST_INIT(ast_translation_unit_t);

	node->external_declaration = NULL;

	if (vector_init(&node->base_type, sizeof(ast_t*), 0))
		goto error_vector_init_base_type;

	if (vector_init(&node->file, sizeof(file_t*), 0))
		goto error_vector_init_file;

	AST_RETURN(AST_TRANSLATION_UNIT);

error_vector_init_file:
	vector_free(&node->file);

error_vector_init_base_type:
	free(node);

	return NULL;
}

void ast_translation_unit_free(ast_t *ast)
{
	AST_FREE(ast_translation_unit_t);

	AST_NODE_FREE(node->external_declaration);

	ast_t **type = node->base_type.buf;
	for (size_t i = 0; i < node->base_type.use; i++)
		AST_NODE_FREE(type[node->base_type.use - i - 1]);

	file_t **file = node->file.buf;
	for (size_t i = 0; i < node->file.use; i++) {
		free(file[i]->path);
		free(file[i]);
	}

	vector_free(&node->base_type);
	vector_free(&node->file);

	free(node);
}

vector_t *ast_translation_unit_get_base_type(
	ast_t *ast)
{
	return &OFFSETOF_AST_NODE(ast, ast_translation_unit_t)->base_type;
}

vector_t *ast_translation_unit_get_file(
	ast_t *ast)
{
	return &OFFSETOF_AST_NODE(ast, ast_translation_unit_t)->file;
}

void fprint_ast_translation_unit(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_translation_unit_t);

	FPRINT_AST_MEMBER("external-declaration", node->external_declaration);

	INDENT(stream, level);
	fprintf(stream, "\"ast-type\" : \"%s\"\n", AST_NODE_STR(&node->ast));

	FPRINT_AST_FINISH;
}
