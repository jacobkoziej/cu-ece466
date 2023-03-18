// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * and_expression.c -- and_expression ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/and_expression.h>
#include <jkcc/private/ast.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/lexer.h>


ast_t *ast_and_expression_init(
	ast_t      *and_expression,
	ast_t      *equality_expression,
	bool        and,
	location_t *location_start,
	location_t *location_end)
{
	AST_INIT(ast_and_expression_t);

	node->and_expression      = and_expression;
	node->equality_expression = equality_expression;

	node->and = and;

	node->location.file  = location_start->file;
	node->location.start = location_start->start;

	node->location.end = (location_end)
		? location_end->end
		: location_start->end;

	AST_RETURN(AST_AND_EXPRESSION);
}

void ast_and_expression_free(ast_t *ast)
{
	AST_FREE(ast_and_expression_t);

	AST_NODE_FREE(node->and_expression);
	AST_NODE_FREE(node->equality_expression);

	free(node);
}

void fprint_ast_and_expression(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_and_expression_t);

	INDENT(stream, level);
	fprintf(stream, "\"AND-expression\" : ");

	if (node->and_expression)
		FPRINT_AST_NODE(
			stream,
			node->and_expression,
			level + 1,
			AST_PRINT_NO_INDENT_INITIAL |
			AST_PRINT_NO_TRAILING_NEWLINE);
	else
		fprintf(stream, "null");

	fprintf(stream, ",\n");

	INDENT(stream, level);
	fprintf(stream, "\"equality-expression\" : ");

	if (node->equality_expression)
		FPRINT_AST_NODE(
			stream,
			node->equality_expression,
			level + 1,
			AST_PRINT_NO_INDENT_INITIAL |
			AST_PRINT_NO_TRAILING_NEWLINE);
	else
		fprintf(stream, "null");

	fprintf(stream, ",\n");

	INDENT(stream, level);
	fprintf(stream, "\"and\" : %s,\n", (node->and) ? "true" : "false");

	FPRINT_AST_NODE_FINISH;
}
