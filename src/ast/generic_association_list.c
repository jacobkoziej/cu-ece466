// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * generic_association_list.c -- generic_association_list ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/generic_association_list.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/location.h>
#include <jkcc/vector.h>


ast_t *ast_generic_association_list_append(
	ast_t       *generic_association_list,
	ast_t       *generic_association,
	location_t  *location,
	const char **error)
{
	ast_generic_association_list_t *list = OFFSETOF_AST_NODE(
		generic_association_list,
		ast_generic_association_list_t);

	if (
		*generic_association != AST_GENERIC_ASSOCIATION &&
		list->default_association) {
		*error = "duplicate `default` case in `_Generic`";
		return NULL;
	}

	if (vector_append(&list->generic_association, &generic_association))
		return NULL;

	list->default_association =
		*generic_association != AST_GENERIC_ASSOCIATION;

	list->location.end = location->end;

	return generic_association_list;
}

ast_t *ast_generic_association_list_init(
	ast_t      *generic_association,
	location_t *location)
{
	AST_INIT(ast_generic_association_list_t);

	if (vector_init(
		&node->generic_association,
		sizeof(generic_association),
		0)) goto error_init;

	if (vector_append(&node->generic_association, &generic_association))
		goto error_append;

	node->default_association =
		(*generic_association == AST_GENERIC_ASSOCIATION)
		? true
		: false;

	node->location = *location;

	AST_RETURN(AST_GENERIC_ASSOCIATION_LIST);

error_append:
	vector_free(&node->generic_association);

error_init:
	free(node);

	return NULL;
}

void ast_generic_association_list_free(ast_t *ast)
{
	AST_FREE(ast_generic_association_list_t);

	ast_t **generic_association = node->generic_association.buf;

	for (size_t i = 0; i < node->generic_association.use; i++)
		AST_NODE_FREE(generic_association[i]);

	vector_free(&node->generic_association);

	free(node);
}

void fprint_ast_generic_association_list(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_generic_association_list_t);

	FPRINT_AST_LIST(
		ast_node_str[AST_GENERIC_ASSOCIATION],
		node->generic_association);

	FPRINT_AST_NODE_FINISH;
}
