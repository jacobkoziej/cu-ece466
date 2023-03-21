// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * inclusive_or_expression.c -- inclusive_or_expression ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/inclusive_or_expression.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/lexer.h>


ast_t *ast_inclusive_or_expression_init(
	ast_t      *inclusive_or_expression,
	ast_t      *exclusive_or_expression,
	location_t *location_start,
	location_t *location_end)
{
	AST_INIT(ast_inclusive_or_expression_t);

	node->inclusive_or_expression = inclusive_or_expression;
	node->exclusive_or_expression = exclusive_or_expression;

	node->location.file  = location_start->file;
	node->location.start = location_start->start;

	node->location.end = (location_end)
		? location_end->end
		: location_start->end;

	AST_RETURN(AST_INCLUSIVE_OR_EXPRESSION);
}

void ast_inclusive_or_expression_free(ast_t *ast)
{
	AST_FREE(ast_inclusive_or_expression_t);

	AST_NODE_FREE(node->inclusive_or_expression);
	AST_NODE_FREE(node->exclusive_or_expression);

	free(node);
}

void fprint_ast_inclusive_or_expression(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_inclusive_or_expression_t);

	FPRINT_AST_MEMBER(
		ast_node_str[AST_INCLUSIVE_OR_EXPRESSION],
		node->inclusive_or_expression);
	FPRINT_AST_MEMBER(
		ast_node_str[AST_EXCLUSIVE_OR_EXPRESSION],
		node->exclusive_or_expression);

	FPRINT_AST_NODE_FINISH;
}
