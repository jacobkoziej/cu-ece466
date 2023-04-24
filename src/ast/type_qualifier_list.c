// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * type_qualifier_list.c -- type_qualifier_list ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast/ast.h>
#include <jkcc/ast/type_qualifier_list.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <jkcc/location.h>
#include <jkcc/vector.h>


ast_t *ast_type_qualifier_list_append(
	ast_t      *type_qualifier_list,
	ast_t      *type_qualifier,
	location_t *location)
{
	ast_type_qualifier_list_t *node = OFFSETOF_AST_NODE(
		type_qualifier_list,
		ast_type_qualifier_list_t);

	if (vector_append(&node->type_qualifier, &type_qualifier))
		return NULL;

	node->location.end = location->end;

	return type_qualifier_list;
}

ast_t *ast_type_qualifier_list_init(
	ast_t      *type_qualifier,
	location_t *location)
{
	AST_INIT(ast_type_qualifier_list_t);

	if (vector_init(&node->type_qualifier, sizeof(type_qualifier), 0))
		goto error_init;

	if (vector_append(&node->type_qualifier, &type_qualifier))
		goto error_append;

	node->location = *location;

	AST_RETURN(AST_TYPE_QUALIFIER_LIST);

error_append:
	vector_free(&node->type_qualifier);

error_init:
	free(node);

	return NULL;
}

void ast_type_qualifier_list_free(ast_t *ast)
{
	AST_FREE(ast_type_qualifier_list_t);

	ast_t **type_qualifier = node->type_qualifier.buf;

	for (size_t i = 0; i < node->type_qualifier.use; i++)
		AST_NODE_FREE(type_qualifier[i]);

	vector_free(&node->type_qualifier);

	free(node);
}

void fprint_ast_type_qualifier_list(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_type_qualifier_list_t);

	FPRINT_AST_LIST(
		ast_node_str[AST_TYPE_QUALIFIER],
		node->type_qualifier);

	FPRINT_AST_NODE_FINISH;
}
