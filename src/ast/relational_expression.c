// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * relational_expression.c -- relational_expression ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/relational_expression.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/lexer.h>


ast_t *ast_relational_expression_init(
	ast_t        *relational_expression,
	ast_t        *shift_expression,
	uint_fast8_t  relation,
	location_t   *location_start,
	location_t   *location_end)
{
	AST_INIT(ast_relational_expression_t);

	node->relational_expression = relational_expression;
	node->shift_expression      = shift_expression;

	node->relation = relation;

	node->location.file  = location_start->file;
	node->location.start = location_start->start;

	node->location.end = (location_end)
		? location_end->end
		: location_start->end;

	AST_RETURN(AST_RELATIONAL_EXPRESSION);
}

void ast_relational_expression_free(ast_t *ast)
{
	AST_FREE(ast_relational_expression_t);

	AST_NODE_FREE(node->relational_expression);
	AST_NODE_FREE(node->shift_expression);

	free(node);
}

void fprint_ast_relational_expression(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_relational_expression_t);

	INDENT(stream, level);
	fprintf(stream, "\"relational-expression\" : ");

	if (node->relational_expression)
		FPRINT_AST_NODE(
			stream,
			node->relational_expression,
			level + 1,
			AST_PRINT_NO_INDENT_INITIAL |
			AST_PRINT_NO_TRAILING_NEWLINE);
	else
		fprintf(stream, "null");

	fprintf(stream, ",\n");

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

	const char *relation;
	switch (node->relation) {
		case 0:
			relation = "";
			break;

		case RELATIONAL_EXPRESSION_LESS_THAN:
			relation = "<";
			break;

		case RELATIONAL_EXPRESSION_GREATER_THAN:
			relation = ">";
			break;

		case RELATIONAL_EXPRESSION_LESS_THAN_OR_EQUAL:
			relation = "<=";
			break;

		case RELATIONAL_EXPRESSION_GREATER_THAN_OR_EQUAL:
			relation = ">=";
			break;

		default:
			relation = "(unknown)";
			break;
	}

	INDENT(stream, level);
	fprintf(stream, "\"relation\" : \"%s\",\n", relation);

	FPRINT_AST_NODE_FINISH;
}
