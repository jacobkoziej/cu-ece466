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

#include "y.tab.h"


ast_t *ast_unary_operator_init(int type, location_t *location)
{
	AST_INIT(ast_unary_operator_t);

	node->type     =  type;
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

	const char *type;
	switch (node->type) {
		case PUNCTUATOR_AMPERSAND:
			type = "&";
			break;

		case PUNCTUATOR_ASTERISK:
			type = "*";
			break;

		case PUNCTUATOR_PLUS:
			type = "+";
			break;

		case PUNCTUATOR_MINUS:
			type = "-";
			break;

		case PUNCTUATOR_UNARY_COMPLEMENT:
			type = "~";
			break;

		case PUNCTUATOR_LOGICAL_NOT:
			type = "!";
			break;

		default:
			type = "(unknown)";
			break;
	}

	INDENT(stream, level);
	fprintf(stream, "\"type\" : \"%s\",\n", type);

	FPRINT_AST_NODE_FINISH;
}
