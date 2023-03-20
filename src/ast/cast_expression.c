// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * cast_expression.c -- cast_expression ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/cast_expression.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/lexer.h>


ast_t *ast_cast_expression_init(
	ast_t        *cast_expression,
	ast_t        *type_name,
	location_t   *location_start,
	location_t   *location_end)
{
	AST_INIT(ast_cast_expression_t);

	node->cast_expression  = cast_expression;
	node->type_name        = type_name;

	node->location.file  = location_start->file;
	node->location.start = location_start->start;

	node->location.end = (location_end)
		? location_end->end
		: location_start->end;

	AST_RETURN(AST_CAST_EXPRESSION);
}

void ast_cast_expression_free(ast_t *ast)
{
	AST_FREE(ast_cast_expression_t);

	AST_NODE_FREE(node->cast_expression);
	AST_NODE_FREE(node->type_name);

	free(node);
}

void fprint_ast_cast_expression(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_cast_expression_t);

	FPRINT_AST_MEMBER(node->cast_expression);
	FPRINT_AST_MEMBER(node->type_name);

	FPRINT_AST_NODE_FINISH;
}
