// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * inclusive_or_expression.c -- inclusive_or_expression ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/inclusive_or_expression.h>
#include <jkcc/private/ast.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/lexer.h>


ast_t *ast_inclusive_or_expression_init(
	ast_t      *inclusive_or_expression,
	ast_t      *exclusive_or_expression,
	bool        inclusive_or,
	location_t *location_start,
	location_t *location_end)
{
	AST_INIT(ast_inclusive_or_expression_t);

	node->inclusive_or_expression = inclusive_or_expression;
	node->exclusive_or_expression = exclusive_or_expression;

	node->inclusive_or = inclusive_or;

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
	fprintf(stream, "\"exclusive-OR-expression\" : ");

	if (node->exclusive_or_expression)
		FPRINT_AST_NODE(
			stream,
			node->exclusive_or_expression,
			level + 1,
			AST_PRINT_NO_INDENT_INITIAL |
			AST_PRINT_NO_TRAILING_NEWLINE);
	else
		fprintf(stream, "null");

	fprintf(stream, ",\n");

	INDENT(stream, level);
	fprintf(
		stream,
		"\"inclusive_or\" : %s,\n",
		(node->inclusive_or) ? "true" : "false");

	FPRINT_AST_NODE_FINISH;
}
