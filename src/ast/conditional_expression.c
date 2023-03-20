// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * conditional_expression.c -- conditional_expression ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/conditional_expression.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/lexer.h>


ast_t *ast_conditional_expression_init(
	ast_t      *logical_or_expression,
	ast_t      *expression,
	ast_t      *conditional_expression,
	location_t *location_start,
	location_t *location_end)
{
	AST_INIT(ast_conditional_expression_t);

	node->logical_or_expression   = logical_or_expression;
	node->expression              = expression;
	node->conditional_expression  = conditional_expression;

	node->location.file  = location_start->file;
	node->location.start = location_start->start;

	node->location.end = (location_end)
		? location_end->end
		: location_start->end;

	AST_RETURN(AST_CONDITIONAL_EXPRESSION);
}

void ast_conditional_expression_free(ast_t *ast)
{
	AST_FREE(ast_conditional_expression_t);

	AST_NODE_FREE(node->logical_or_expression);
	AST_NODE_FREE(node->expression);
	AST_NODE_FREE(node->conditional_expression);

	free(node);
}

void fprint_ast_conditional_expression(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_conditional_expression_t);

	FPRINT_AST_MEMBER(node->logical_or_expression);
	FPRINT_AST_MEMBER(node->expression);
	FPRINT_AST_MEMBER(node->conditional_expression);

	FPRINT_AST_NODE_FINISH;
}
