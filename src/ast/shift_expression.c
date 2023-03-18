// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * shift_expression.c -- shift_expression ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/shift_expression.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/lexer.h>


ast_t *ast_shift_expression_init(
	ast_t        *shift_expression,
	ast_t        *additive_expression,
	uint_fast8_t  operation,
	location_t   *location_start,
	location_t   *location_end)
{
	AST_INIT(ast_shift_expression_t);

	node->shift_expression    = shift_expression;
	node->additive_expression = additive_expression;

	node->operation = operation;

	node->location.file  = location_start->file;
	node->location.start = location_start->start;

	node->location.end = (location_end)
		? location_end->end
		: location_start->end;

	AST_RETURN(AST_SHIFT_EXPRESSION);
}

void ast_shift_expression_free(ast_t *ast)
{
	AST_FREE(ast_shift_expression_t);

	AST_NODE_FREE(node->shift_expression);
	AST_NODE_FREE(node->additive_expression);

	free(node);
}

void fprint_ast_shift_expression(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_shift_expression_t);

	INDENT(stream, level);
	fprintf(stream, "\"shift-expression\" : ");

	if (node->shift_expression)
		FPRINT_AST_NODE(
			stream,
			node->shift_expression,
			level + 1,
			AST_PRINT_NO_INDENT_INITIAL |
			AST_PRINT_NO_TRAILING_NEWLINE);
	else
		fprintf(stream, "null");

	fprintf(stream, ",\n");

	INDENT(stream, level);
	fprintf(stream, "\"additive-expression\" : ");

	if (node->additive_expression)
		FPRINT_AST_NODE(
			stream,
			node->additive_expression,
			level + 1,
			AST_PRINT_NO_INDENT_INITIAL |
			AST_PRINT_NO_TRAILING_NEWLINE);
	else
		fprintf(stream, "null");

	fprintf(stream, ",\n");

	const char *operation;
	switch (node->operation) {
		case 0:
			operation = "";
			break;

		case SHIFT_EXPRESSION_LBITSHIFT:
			operation = "<<";
			break;

		case SHIFT_EXPRESSION_RBITSHIFT:
			operation = ">>";
			break;

		default:
			operation = "(unknown)";
			break;
	}

	INDENT(stream, level);
	fprintf(stream, "\"operation\" : \"%s\",\n", operation);

	FPRINT_AST_NODE_FINISH;
}
