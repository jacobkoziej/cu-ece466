// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * init_declarator_list.c -- init_declarator_list ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/init_declarator_list.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/location.h>
#include <jkcc/vector.h>

#include "y.tab.h"


ast_t *ast_init_declarator_list_append(
	ast_t      *init_declarator_list,
	ast_t      *init_declarator,
	location_t *location)
{
	ast_init_declarator_list_t *node = OFFSETOF_AST_NODE(
		init_declarator_list,
		ast_init_declarator_list_t);

	if (vector_append(&node->init_declarator, &init_declarator))
		return NULL;

	node->location.end = location->end;

	return init_declarator_list;
}

ast_t *ast_init_declarator_list_init(
	ast_t      *init_declarator_list,
	ast_t      *init_declarator,
	location_t *location_start,
	location_t *location_end)
{
	AST_INIT(ast_init_declarator_list_t);

	if (vector_init(&node->init_declarator, sizeof(init_declarator_list), 0))
		goto error_init;

	if (vector_append(&node->init_declarator, &init_declarator_list))
		goto error_append;

	if (vector_append(&node->init_declarator, &init_declarator))
		goto error_append;

	AST_NODE_LOCATION;

	AST_RETURN(AST_INIT_DECLARATOR_LIST);

error_append:
	vector_free(&node->init_declarator);

error_init:
	return NULL;
}

void ast_init_declarator_list_free(ast_t *ast)
{
	AST_FREE(ast_init_declarator_list_t);

	ast_t **init_declarator = node->init_declarator.buf;

	for (size_t i = 0; i < node->init_declarator.use; i++)
		AST_NODE_FREE(init_declarator[i]);

	vector_free(&node->init_declarator);

	free(node);
}

void fprint_ast_init_declarator_list(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_init_declarator_list_t);

	FPRINT_AST_LIST("init-declarator", node->init_declarator);

	FPRINT_AST_NODE_FINISH;
}
