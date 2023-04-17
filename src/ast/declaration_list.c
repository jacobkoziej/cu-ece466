// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * declaration_list.c -- declaration_list ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/declaration_list.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/location.h>
#include <jkcc/vector.h>


ast_t *ast_declaration_list_append(
	ast_t      *declaration_list,
	ast_t      *declaration,
	location_t *location)
{
	ast_declaration_list_t *node = OFFSETOF_AST_NODE(
		declaration_list,
		ast_declaration_list_t);

	if (vector_append(&node->declaration, &declaration))
		return NULL;

	node->location.end = location->end;

	return declaration_list;
}

ast_t *ast_declaration_list_init(
	ast_t      *declaration_list,
	ast_t      *declaration,
	location_t *location_start,
	location_t *location_end)
{
	AST_INIT(ast_declaration_list_t);

	if (vector_init(&node->declaration, sizeof(declaration_list), 0))
		goto error_init;

	if (declaration_list)
		if (vector_append(&node->declaration, &declaration_list))
			goto error_append;

	if (vector_append(&node->declaration, &declaration))
		goto error_append;

	AST_NODE_LOCATION;

	AST_RETURN(AST_DECLARATION_LIST);

error_append:
	vector_free(&node->declaration);

error_init:
	return NULL;
}

void ast_declaration_list_free(ast_t *ast)
{
	AST_FREE(ast_declaration_list_t);

	ast_t **declaration = node->declaration.buf;

	for (size_t i = 0; i < node->declaration.use; i++)
		AST_NODE_FREE(declaration[i]);

	vector_free(&node->declaration);

	free(node);
}

void fprint_ast_declaration_list(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_declaration_list_t);

	FPRINT_AST_LIST(ast_node_str[AST_DECLARATION], node->declaration);

	FPRINT_AST_NODE_FINISH;
}
