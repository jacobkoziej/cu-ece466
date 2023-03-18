// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * logical_and_expression.c -- logical_and_expression ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/logical_and_expression.h>
#include <jkcc/private/ast.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/lexer.h>


ast_t *ast_logical_and_expression_init(
	ast_t      *logical_and_expression,
	ast_t      *inclusive_or_expression,
	bool        logical_and,
	location_t *location_start,
	location_t *location_end)
{
	AST_INIT(ast_logical_and_expression_t);

	node->logical_and_expression  = logical_and_expression;
	node->inclusive_or_expression = inclusive_or_expression;

	node->logical_and = logical_and;

	node->location.file  = location_start->file;
	node->location.start = location_start->start;

	node->location.end = (location_end)
		? location_end->end
		: location_start->end;

	AST_RETURN(AST_LOGICAL_AND_EXPRESSION);
}

void ast_logical_and_expression_free(ast_t *ast)
{
	AST_FREE(ast_logical_and_expression_t);

	AST_NODE_FREE(node->logical_and_expression);
	AST_NODE_FREE(node->inclusive_or_expression);

	free(node);
}

void fprint_ast_logical_and_expression(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_logical_and_expression_t);

	INDENT(stream, level);
	fprintf(stream, "\"logical-AND-expression\" : ");

	if (node->logical_and_expression)
		FPRINT_AST_NODE(
			stream,
			node->logical_and_expression,
			level + 1,
			AST_PRINT_NO_INDENT_INITIAL |
			AST_PRINT_NO_TRAILING_NEWLINE);
	else
		fprintf(stream, "null");

	fprintf(stream, ",\n");

	INDENT(stream, level);
	fprintf(stream, "\"inclusive-OR-expression\" : ");

	if (node->inclusive_or_expression)
		FPRINT_AST_NODE(
			stream,
			node->inclusive_or_expression,
			level + 1,
			AST_PRINT_NO_INDENT_INITIAL |
			AST_PRINT_NO_TRAILING_NEWLINE);
	else
		fprintf(stream, "null");

	fprintf(stream, ",\n");

	INDENT(stream, level);
	fprintf(
		stream,
		"\"logical_and\" : %s,\n",
		(node->logical_and) ? "true" : "false");

	FPRINT_AST_NODE_FINISH;
}
