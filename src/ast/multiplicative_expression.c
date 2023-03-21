// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * multiplicative_expression.c -- multiplicative_expression ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/multiplicative_expression.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/lexer.h>


ast_t *ast_multiplicative_expression_init(
	ast_t        *multiplicative_expression,
	ast_t        *cast_expression,
	uint_fast8_t  operation,
	location_t   *location_start,
	location_t   *location_end)
{
	AST_INIT(ast_multiplicative_expression_t);

	node->multiplicative_expression = multiplicative_expression;
	node->cast_expression           = cast_expression;

	node->operation = operation;

	node->location.file  = location_start->file;
	node->location.start = location_start->start;

	node->location.end = (location_end)
		? location_end->end
		: location_start->end;

	AST_RETURN(AST_MULTIPLICATIVE_EXPRESSION);
}

void ast_multiplicative_expression_free(ast_t *ast)
{
	AST_FREE(ast_multiplicative_expression_t);

	AST_NODE_FREE(node->multiplicative_expression);
	AST_NODE_FREE(node->cast_expression);

	free(node);
}

void fprint_ast_multiplicative_expression(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_multiplicative_expression_t);

	FPRINT_AST_MEMBER(
		ast_node_str[AST_MULTIPLICATIVE_EXPRESSION],
		node->multiplicative_expression);
	FPRINT_AST_MEMBER(
		ast_node_str[AST_CAST_EXPRESSION],
		node->cast_expression);

	const char *operation;
	switch (node->operation) {
		case 0:
			operation = "";
			break;

		case MULTIPLICATIVE_EXPRESSION_MULTIPLICATION:
			operation = "*";
			break;

		case MULTIPLICATIVE_EXPRESSION_DIVISION:
			operation = "/";
			break;

		case MULTIPLICATIVE_EXPRESSION_MODULO:
			operation = "%";
			break;

		default:
			operation = "(unknown)";
			break;
	}

	INDENT(stream, level);
	fprintf(stream, "\"operation\" : \"%s\",\n", operation);

	FPRINT_AST_NODE_FINISH;
}
