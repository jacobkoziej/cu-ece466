// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * return.c -- return ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast/ast.h>
#include <jkcc/ast/return.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/location.h>


ast_t *ast_return_init(
	ast_t      *expression,
	location_t *location_start,
	location_t *location_end)
{
	AST_INIT(ast_return_t);

	node->expression = expression;

	AST_NODE_LOCATION;

	AST_RETURN(AST_RETURN);
}

void ast_return_free(ast_t *ast)
{
	AST_FREE(ast_return_t);

	AST_NODE_FREE(node->expression);

	free(node);
}

ast_t *ast_return_get_expression(ast_t *ast)
{
	return OFFSETOF_AST_NODE(ast, ast_return_t)->expression;
}

void fprint_ast_return(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_return_t);

	FPRINT_AST_MEMBER(ast_node_str[AST_EXPRESSION], node->expression);

	FPRINT_AST_NODE_FINISH;
}
