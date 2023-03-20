// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * unary_expression.c -- unary_expression ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/unary_expression.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/lexer.h>


ast_t *ast_unary_expression_init(
	ast_t        *unary_expression,
	ast_t        *unary_operator,
	ast_t        *cast_expression,
	ast_t        *type_name,
	uint_fast8_t  flags,
	location_t   *location_start,
	location_t   *location_end)
{
	AST_INIT(ast_unary_expression_t);

	node->unary_expression   = unary_expression;
	node->unary_operator     = unary_operator;
	node->cast_expression    = cast_expression;
	node->type_name          = type_name;

	node->flags = flags;

	node->location.file  = location_start->file;
	node->location.start = location_start->start;

	node->location.end = (location_end)
		? location_end->end
		: location_start->end;

	AST_RETURN(AST_UNARY_EXPRESSION);
}

void ast_unary_expression_free(ast_t *ast)
{
	AST_FREE(ast_unary_expression_t);

	AST_NODE_FREE(node->unary_expression);
	AST_NODE_FREE(node->unary_operator);
	AST_NODE_FREE(node->cast_expression);
	AST_NODE_FREE(node->type_name);

	free(node);
}

void fprint_ast_unary_expression(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_unary_expression_t);

	INDENT(stream, level);
	fprintf(
		stream,
		"\"%s\" : ",
		AST_NODE_STR(node->unary_expression));

	FPRINT_AST_NODE(
		stream,
		node->unary_expression,
		level + 1,
		AST_PRINT_NO_INDENT_INITIAL |
		AST_PRINT_NO_TRAILING_NEWLINE);
	fprintf(stream, ",\n");

	INDENT(stream, level);
	fprintf(stream, "\"unary-operator\" : ");

	if (node->unary_operator)
		FPRINT_AST_NODE(
			stream,
			node->unary_operator,
			level + 1,
			AST_PRINT_NO_INDENT_INITIAL |
			AST_PRINT_NO_TRAILING_NEWLINE);
	else
		fprintf(stream, "null");

	fprintf(stream, ",\n");

	INDENT(stream, level);
	fprintf(stream, "\"cast-expression\" : ");

	if (node->cast_expression)
		FPRINT_AST_NODE(
			stream,
			node->cast_expression,
			level + 1,
			AST_PRINT_NO_INDENT_INITIAL |
			AST_PRINT_NO_TRAILING_NEWLINE);
	else
		fprintf(stream, "null");

	fprintf(stream, ",\n");

	INDENT(stream, level);
	fprintf(stream, "\"type-name\" : ");

	if (node->type_name)
		FPRINT_AST_NODE(
			stream,
			node->type_name,
			level + 1,
			AST_PRINT_NO_INDENT_INITIAL |
			AST_PRINT_NO_TRAILING_NEWLINE);
	else
		fprintf(stream, "null");

	fprintf(stream, ",\n");

	INDENT(stream, level);
	fprintf(
		stream,
		"\"increment\" : %s,\n",
		(node->flags & UNARY_EXPRESSION_INCREMENT)
		? "true"
		: "false");

	INDENT(stream, level);
	fprintf(
		stream,
		"\"decrement\" : %s,\n",
		(node->flags & UNARY_EXPRESSION_DECREMENT)
		? "true"
		: "false");

	INDENT(stream, level);
	fprintf(
		stream,
		"\"sizeof\" : %s,\n",
		(node->flags & UNARY_EXPRESSION_SIZEOF)
		? "true"
		: "false");

	INDENT(stream, level);
	fprintf(
		stream,
		"\"_Alignof\" : %s,\n",
		(node->flags & UNARY_EXPRESSION__ALIGNOF)
		? "true"
		: "false");

	FPRINT_AST_NODE_FINISH;
}
