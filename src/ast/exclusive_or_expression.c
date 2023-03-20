// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * exclusive_or_expression.c -- exclusive_or_expression ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/exclusive_or_expression.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/lexer.h>


ast_t *ast_exclusive_or_expression_init(
	ast_t      *exclusive_or_expression,
	ast_t      *and_expression,
	location_t *location_start,
	location_t *location_end)
{
	AST_INIT(ast_exclusive_or_expression_t);

	node->exclusive_or_expression = exclusive_or_expression;
	node->and_expression          = and_expression;

	node->location.file  = location_start->file;
	node->location.start = location_start->start;

	node->location.end = (location_end)
		? location_end->end
		: location_start->end;

	AST_RETURN(AST_EXCLUSIVE_OR_EXPRESSION);
}

void ast_exclusive_or_expression_free(ast_t *ast)
{
	AST_FREE(ast_exclusive_or_expression_t);

	AST_NODE_FREE(node->exclusive_or_expression);
	AST_NODE_FREE(node->and_expression);

	free(node);
}

void fprint_ast_exclusive_or_expression(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_exclusive_or_expression_t);

	FPRINT_AST_MEMBER(node->exclusive_or_expression);
	FPRINT_AST_MEMBER(node->and_expression);

	FPRINT_AST_NODE_FINISH;
}
