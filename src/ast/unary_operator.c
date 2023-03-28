// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * unary_operator.c -- unary_operator ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/unary_operator.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/lexer.h>


ast_t *ast_unary_operator_init(
	ast_t        *operand,
	uint_fast8_t  operator,
	location_t   *location_start,
	location_t   *location_end)
{
	AST_INIT(ast_unary_operator_t);

	node->operand  = operand;
	node->operator = operator;

	node->location.file  = location_start->file;
	node->location.start = location_start->start;

	node->location.end = (location_end)
		? location_end->end
		: location_start->end;

	AST_RETURN(AST_UNARY_OPERATOR);
}

void ast_unary_operator_free(ast_t *ast)
{
	AST_FREE(ast_unary_operator_t);

	AST_NODE_FREE(node->operand);

	free(node);
}

void fprint_ast_unary_operator(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_unary_operator_t);

	FPRINT_AST_MEMBER("operand", node->operand);

	const char *operator;
	switch (node->operator) {
		case AST_UNARY_OPERATOR_AMPERSAND:
			operator = "&";
			break;

		case AST_UNARY_OPERATOR_ASTERISK:
			operator = "*";
			break;

		case AST_UNARY_OPERATOR_PLUS:
			operator = "+";
			break;

		case AST_UNARY_OPERATOR_MINUS:
			operator = "-";
			break;

		case AST_UNARY_OPERATOR_UNARY_COMPLEMENT:
			operator = "~";
			break;

		case AST_UNARY_OPERATOR_LOGICAL_NOT:
			operator = "!";
			break;

		case AST_UNARY_OPERATOR_INCREMENT:
			operator = "++";
			break;

		case AST_UNARY_OPERATOR_DECREMENT:
			operator = "--";
			break;

		default:
			operator = "(unknown)";
			break;
	}

	INDENT(stream, level);
	fprintf(stream, "\"operator\" : \"%s\",\n", operator);

	FPRINT_AST_NODE_FINISH;
}
