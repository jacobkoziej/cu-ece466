// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * list.c -- list ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast/ast.h>
#include <jkcc/ast/list.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <jkcc/location.h>
#include <jkcc/vector.h>


ast_t *ast_list_append(
	ast_t      *list,
	ast_t      *ast,
	location_t *location)
{
	ast_list_t *node = OFFSETOF_AST_NODE(
		list,
		ast_list_t);

	if (vector_append(&node->list, &ast))
		return NULL;

	node->location.end = location->end;

	return list;
}

ast_t *ast_list_init(
	ast_t      *ast,
	location_t *location)
{
	AST_INIT(ast_list_t);

	if (vector_init(&node->list, sizeof(ast), 0))
		goto error_init;

	if (vector_append(&node->list, &ast))
		goto error_append;

	node->location = *location;

	AST_RETURN(AST_LIST);

error_append:
	vector_free(&node->list);

error_init:
	free(node);

	return NULL;
}

void ast_list_free(ast_t *ast)
{
	AST_FREE(ast_list_t);

	ast_t **item = node->list.buf;

	for (size_t i = 0; i < node->list.use; i++)
		AST_NODE_FREE(item[i]);

	vector_free(&node->list);

	free(node);
}

void fprint_ast_list(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_list_t);

	FPRINT_AST_LIST(ast_node_str[AST_LIST], node->list);

	FPRINT_AST_NODE_FINISH;
}
