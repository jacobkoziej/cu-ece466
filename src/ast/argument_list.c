// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * argument_list.c -- argument_list ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/argument_list.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <jkcc/location.h>
#include <jkcc/vector.h>


ast_t *ast_argument_list_append(
	ast_t      *argument_list,
	ast_t      *argument,
	location_t *location)
{
	ast_argument_list_t *node = OFFSETOF_AST_NODE(
		argument_list,
		ast_argument_list_t);

	if (vector_append(&node->argument, &argument))
		return NULL;

	node->location.end = location->end;

	return argument_list;
}

ast_t *ast_argument_list_init(
	ast_t      *argument,
	location_t *location)
{
	AST_INIT(ast_argument_list_t);

	if (vector_init(&node->argument, sizeof(argument), 0))
		goto error_init;

	if (vector_append(&node->argument, &argument))
		goto error_append;

	node->location = *location;

	AST_RETURN(AST_ARGUMENT_LIST);

error_append:
	vector_free(&node->argument);

error_init:
	free(node);

	return NULL;
}

void ast_argument_list_free(ast_t *ast)
{
	AST_FREE(ast_argument_list_t);

	ast_t **argument = node->argument.buf;

	for (size_t i = 0; i < node->argument.use; i++)
		AST_NODE_FREE(argument[i]);

	vector_free(&node->argument);

	free(node);
}

void fprint_ast_argument_list(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_argument_list_t);

	FPRINT_AST_LIST("argument", node->argument);

	FPRINT_AST_NODE_FINISH;
}
