// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * assignment_operator.c -- assignment_operator ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/assignment_operator.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/lexer.h>

#include "y.tab.h"


ast_t *ast_assignment_operator_init(
	uint_fast16_t  operator,
	location_t    *location)
{
	AST_INIT(ast_assignment_operator_t);

	node->operator =  operator;
	node->location = *location;

	AST_RETURN(AST_ASSIGNMENT_OPERATOR);
}

void ast_assignment_operator_free(ast_t *ast)
{
	AST_FREE(ast_assignment_operator_t);

	free(node);
}

void fprint_ast_assignment_operator(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_assignment_operator_t);

	const char *operator;
	switch (node->operator) {
		case ASSIGNMENT_OPERATOR_ASSIGNMENT:
			operator = "=";
			break;

		case ASSIGNMENT_OPERATOR_COMPOUND_MULTIPLICATION:
			operator = "*=";
			break;

		case ASSIGNMENT_OPERATOR_COMPOUND_DIVISION:
			operator = "/=";
			break;

		case ASSIGNMENT_OPERATOR_COMPOUND_MODULO:
			operator = "%=";
			break;

		case ASSIGNMENT_OPERATOR_COMPOUND_ADDITION:
			operator = "+=";
			break;

		case ASSIGNMENT_OPERATOR_COMPOUND_SUBTRACTION:
			operator = "-=";
			break;

		case ASSIGNMENT_OPERATOR_COMPOUND_LBITSHIFT:
			operator = "<<=";
			break;

		case ASSIGNMENT_OPERATOR_COMPOUND_RBITSHIFT:
			operator = ">>=";
			break;

		case ASSIGNMENT_OPERATOR_COMPOUND_AND:
			operator = "&=";
			break;

		case ASSIGNMENT_OPERATOR_COMPOUND_XOR:
			operator = "^=";
			break;

		case ASSIGNMENT_OPERATOR_COMPOUND_OR:
			operator = "|=";
			break;

		default:
			operator = "(unknown)";
			break;
	}

	INDENT(stream, level);
	fprintf(stream, "\"operator\" : \"%s\",\n", operator);

	FPRINT_AST_NODE_FINISH;
}
