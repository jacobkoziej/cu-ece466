// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * cast.c -- cast ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/cast.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/location.h>


ast_t *ast_cast_init(
	ast_t        *expression,
	ast_t        *type,
	location_t   *location_start,
	location_t   *location_end)
{
	AST_INIT(ast_cast_t);

	node->expression = expression;
	node->type       = type;

	AST_NODE_LOCATION;

	AST_RETURN(AST_CAST);
}

void ast_cast_free(ast_t *ast)
{
	AST_FREE(ast_cast_t);

	AST_NODE_FREE(node->expression);
	AST_NODE_FREE(node->type);

	free(node);
}

void fprint_ast_cast(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_cast_t);

	FPRINT_AST_MEMBER(ast_node_str[AST_EXPRESSION], node->expression);
	FPRINT_AST_MEMBER("type", node->type);

	FPRINT_AST_NODE_FINISH;
}
