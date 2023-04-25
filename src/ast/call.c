// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * call.c -- call ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast/ast.h>
#include <jkcc/ast/call.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/location.h>


ast_t *ast_call_init(
	ast_t        *expression,
	ast_t        *argument_list,
	location_t   *location_start,
	location_t   *location_end)
{
	AST_INIT(ast_call_t);

	node->expression    = expression;
	node->argument_list = argument_list;

	AST_NODE_LOCATION;

	AST_RETURN(AST_CALL);
}

void ast_call_free(ast_t *ast)
{
	AST_FREE(ast_call_t);

	AST_NODE_FREE(node->expression);
	AST_NODE_FREE(node->argument_list);

	free(node);
}

void fprint_ast_call(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_call_t);

	FPRINT_AST_MEMBER(ast_node_str[AST_EXPRESSION], node->expression);
	FPRINT_AST_MEMBER("argument-list", node->argument_list);

	FPRINT_AST_NODE_FINISH;
}
