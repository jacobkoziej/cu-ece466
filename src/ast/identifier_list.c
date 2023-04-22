// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * identifier_list.c -- identifier_list ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast/ast.h>
#include <jkcc/ast/identifier_list.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <jkcc/location.h>
#include <jkcc/vector.h>


ast_t *ast_identifier_list_append(
	ast_t      *identifier_list,
	ast_t      *identifier,
	location_t *location)
{
	ast_identifier_list_t *node = OFFSETOF_AST_NODE(
		identifier_list,
		ast_identifier_list_t);

	if (vector_append(&node->identifier, &identifier))
		return NULL;

	node->location.end = location->end;

	return identifier_list;
}

ast_t *ast_identifier_list_init(
	ast_t      *identifier,
	location_t *location)
{
	AST_INIT(ast_identifier_list_t);

	if (vector_init(&node->identifier, sizeof(identifier), 0))
		goto error_init;

	if (vector_append(&node->identifier, &identifier))
		goto error_append;

	node->location = *location;

	AST_RETURN(AST_IDENTIFIER_LIST);

error_append:
	vector_free(&node->identifier);

error_init:
	free(node);

	return NULL;
}

void ast_identifier_list_free(ast_t *ast)
{
	AST_FREE(ast_identifier_list_t);

	ast_t **identifier = node->identifier.buf;

	for (size_t i = 0; i < node->identifier.use; i++)
		AST_NODE_FREE(identifier[i]);

	vector_free(&node->identifier);

	free(node);
}

void fprint_ast_identifier_list(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_identifier_list_t);

	FPRINT_AST_LIST(ast_node_str[AST_IDENTIFIER], node->identifier);

	FPRINT_AST_NODE_FINISH;
}
