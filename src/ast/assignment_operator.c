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
#include <jkcc/string.h>

#include "y.tab.h"


ast_t *ast_assignment_operator_init(int type, location_t *location)
{
	AST_INIT(ast_assignment_operator_t);

	node->type     =  type;
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

	const char *type;
	switch (node->type) {
		case PUNCTUATOR_ASSIGNMENT:
			type = "=";
			break;

		case PUNCTUATOR_COMPOUND_MULTIPLICATION:
			type = "*=";
			break;

		case PUNCTUATOR_COMPOUND_DIVISION:
			type = "/=";
			break;

		case PUNCTUATOR_COMPOUND_MODULO:
			type = "%=";
			break;

		case PUNCTUATOR_COMPOUND_ADDITION:
			type = "+=";
			break;

		case PUNCTUATOR_COMPOUND_SUBTRACTION:
			type = "-=";
			break;

		case PUNCTUATOR_COMPOUND_LBITSHIFT:
			type = "<<=";
			break;

		case PUNCTUATOR_COMPOUND_RBITSHIFT:
			type = ">>=";
			break;

		case PUNCTUATOR_COMPOUND_AND:
			type = "&=";
			break;

		case PUNCTUATOR_COMPOUND_XOR:
			type = "^=";
			break;

		case PUNCTUATOR_COMPOUND_OR:
			type = "|=";
			break;

		default:
			type = "(unknown)";
			break;
	}

	INDENT(stream, level);
	fprintf(stream, "\"type\" : \"%s\",\n", type);

	FPRINT_AST_NODE_FINISH;
}
