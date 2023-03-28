// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * binary_operator.c -- binary_operator ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/binary_operator.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/lexer.h>


ast_t *ast_binary_operator_init(
	ast_t         *lhs,
	ast_t         *rhs,
	uint_fast32_t  operator,
	location_t    *location_start,
	location_t    *location_end)
{
	AST_INIT(ast_binary_operator_t);

	node->lhs = lhs;
	node->rhs = rhs;

	node->operator = operator;

	node->location.file  = location_start->file;
	node->location.start = location_start->start;

	node->location.end = (location_end)
		? location_end->end
		: location_start->end;

	AST_RETURN(AST_BINARY_OPERATOR);
}

void ast_binary_operator_free(ast_t *ast)
{
	AST_FREE(ast_binary_operator_t);

	AST_NODE_FREE(node->lhs);
	AST_NODE_FREE(node->rhs);

	free(node);
}

void fprint_ast_binary_operator(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_binary_operator_t);

	FPRINT_AST_MEMBER("lhs", node->lhs);
	FPRINT_AST_MEMBER("rhs", node->rhs);

	const char *operator;
	switch (node->operator) {
		case 0:
			operator = "";
			break;

		case AST_BINARY_OPERATOR_MULTIPLICATION:
			operator = "*";
			break;

		case AST_BINARY_OPERATOR_DIVISION:
			operator = "/";
			break;

		case AST_BINARY_OPERATOR_MODULO:
			operator = "%";
			break;

		case AST_BINARY_OPERATOR_ADDITION:
			operator = "+";
			break;

		case AST_BINARY_OPERATOR_SUBTRACTION:
			operator = "-";
			break;

		case AST_BINARY_OPERATOR_LBITSHIFT:
			operator = "<<";
			break;

		case AST_BINARY_OPERATOR_RBITSHIFT:
			operator = ">>";
			break;

		case AST_BINARY_OPERATOR_LESS_THAN:
			operator = "<";
			break;

		case AST_BINARY_OPERATOR_GREATER_THAN:
			operator = ">";
			break;

		case AST_BINARY_OPERATOR_LESS_THAN_OR_EQUAL:
			operator = "<=";
			break;

		case AST_BINARY_OPERATOR_GREATER_THAN_OR_EQUAL:
			operator = ">=";
			break;

		case AST_BINARY_OPERATOR_EQUALITY:
			operator = "==";
			break;

		case AST_BINARY_OPERATOR_INEQUALITY:
			operator = "!=";
			break;

		case AST_BINARY_OPERATOR_AND:
			operator = "&";
			break;

		case AST_BINARY_OPERATOR_EXCLUSIVE_OR:
			operator = "^";
			break;

		case AST_BINARY_OPERATOR_INCLUSIVE_OR:
			operator = "|";
			break;

		case AST_BINARY_OPERATOR_LOGICAL_AND:
			operator = "&&";
			break;

		case AST_BINARY_OPERATOR_LOGICAL_OR:
			operator = "||";
			break;

		default:
			operator = "(unknown)";
			break;
	}

	INDENT(stream, level);
	fprintf(stream, "\"operator\" : \"%s\",\n", operator);

	FPRINT_AST_NODE_FINISH;
}
