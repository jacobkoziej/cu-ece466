// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * equality_expression.c -- equality_expression ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/equality_expression.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/lexer.h>


ast_t *ast_equality_expression_init(
	ast_t        *equality_expression,
	ast_t        *relational_expression,
	uint_fast8_t  equality,
	location_t   *location_start,
	location_t   *location_end)
{
	AST_INIT(ast_equality_expression_t);

	node->equality_expression   = equality_expression;
	node->relational_expression = relational_expression;

	node->equality = equality;

	node->location.file  = location_start->file;
	node->location.start = location_start->start;

	node->location.end = (location_end)
		? location_end->end
		: location_start->end;

	AST_RETURN(AST_EQUALITY_EXPRESSION);
}

void ast_equality_expression_free(ast_t *ast)
{
	AST_FREE(ast_equality_expression_t);

	AST_NODE_FREE(node->equality_expression);
	AST_NODE_FREE(node->relational_expression);

	free(node);
}

void fprint_ast_equality_expression(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_equality_expression_t);

	FPRINT_AST_MEMBER(
		ast_node_str[AST_EQUALITY_EXPRESSION],
		node->equality_expression);
	FPRINT_AST_MEMBER(
		ast_node_str[AST_RELATIONAL_EXPRESSION],
		node->relational_expression);

	const char *equality;
	switch (node->equality) {
		case 0:
			equality = "";
			break;

		case EQUALITY_EXPRESSION_EQUALITY:
			equality = "==";
			break;

		case EQUALITY_EXPRESSION_INEQUALITY:
			equality = "!=";
			break;

		default:
			equality = "(unknown)";
			break;
	}

	INDENT(stream, level);
	fprintf(stream, "\"equality\" : \"%s\",\n", equality);

	FPRINT_AST_NODE_FINISH;
}
