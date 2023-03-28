// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * assignment_expression.c -- assignment_expression ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/assignment_expression.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/lexer.h>


ast_t *ast_assignment_expression_init(
	ast_t        *unary_expression,
	ast_t        *assignment_operator,
	ast_t        *assignment_expression,
	location_t   *location_start,
	location_t   *location_end)
{
	AST_INIT(ast_assignment_expression_t);

	node->unary_expression       = unary_expression;
	node->assignment_operator    = assignment_operator;
	node->assignment_expression  = assignment_expression;

	node->location.file  = location_start->file;
	node->location.start = location_start->start;

	node->location.end = (location_end)
		? location_end->end
		: location_start->end;

	AST_RETURN(AST_ASSIGNMENT_EXPRESSION);
}

void ast_assignment_expression_free(ast_t *ast)
{
	AST_FREE(ast_assignment_expression_t);

	AST_NODE_FREE(node->unary_expression);
	AST_NODE_FREE(node->assignment_operator);
	AST_NODE_FREE(node->assignment_expression);

	free(node);
}

void fprint_ast_assignment_expression(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_assignment_expression_t);

	/* TODO: fix unary_expression deprecation
	FPRINT_AST_MEMBER(
		ast_node_str[AST_UNARY_EXPRESSION],
		node->unary_expression);
	*/
	FPRINT_AST_MEMBER(
		ast_node_str[AST_ASSIGNMENT_OPERATOR],
		node->assignment_operator);
	FPRINT_AST_MEMBER(
		ast_node_str[AST_ASSIGNMENT_EXPRESSION],
		node->assignment_expression);

	FPRINT_AST_NODE_FINISH;
}
