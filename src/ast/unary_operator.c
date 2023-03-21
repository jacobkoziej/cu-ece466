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


ast_t *ast_unary_operator_init(uint_fast8_t operator, location_t *location)
{
	AST_INIT(ast_unary_operator_t);

	node->operator =  operator;
	node->location = *location;

	AST_RETURN(AST_UNARY_OPERATOR);
}

void ast_unary_operator_free(ast_t *ast)
{
	AST_FREE(ast_unary_operator_t);

	free(node);
}

void fprint_ast_unary_operator(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_unary_operator_t);

	const char *operator;
	switch (node->operator) {
		case UNARY_OPERATOR_AMPERSAND:
			operator = "&";
			break;

		case UNARY_OPERATOR_ASTERISK:
			operator = "*";
			break;

		case UNARY_OPERATOR_PLUS:
			operator = "+";
			break;

		case UNARY_OPERATOR_MINUS:
			operator = "-";
			break;

		case UNARY_OPERATOR_UNARY_COMPLEMENT:
			operator = "~";
			break;

		case UNARY_OPERATOR_LOGICAL_NOT:
			operator = "!";
			break;

		default:
			operator = "(unknown)";
			break;
	}

	INDENT(stream, level);
	fprintf(stream, "\"operator\" : \"%s\",\n", operator);

	FPRINT_AST_NODE_FINISH;
}
