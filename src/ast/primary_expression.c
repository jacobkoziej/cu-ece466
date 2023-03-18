// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * primary_expression.c -- primary_expression ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/primary_expression.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/lexer.h>


ast_t *ast_primary_expression_init(
	ast_t      *primary_expression,
	location_t *location)
{
	AST_INIT(ast_primary_expression_t);

	node->primary_expression =  primary_expression;
	node->location           = *location;

	AST_RETURN(AST_PRIMARY_EXPRESSION);
}

void ast_primary_expression_free(ast_t *ast)
{
	AST_FREE(ast_primary_expression_t);

	AST_NODE_FREE(node->primary_expression);

	free(node);
}

void fprint_ast_primary_expression(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_primary_expression_t);

	const char *type;
	switch (*node->primary_expression) {
		case AST_IDENTIFIER:
			type = "identifier";
			break;

		case AST_CONSTANT:
			type = "constant";
			break;

		case AST_STRING_LITERAL:
			type = "string-literal";
			break;

		/* TODO: add support for
		 * expresisons and generic-selection
		case AST_EXPRESSION:
			type = "expression";
			break;

		case AST_GENERIC_SELECTION:
			type = "generic-selection";
			break;
		*/

		default:
			type = "(unknown)";
			break;
	}

	INDENT(stream, level);
	fprintf(stream, "\"%s\" : ", type);

	FPRINT_AST_NODE(
		stream,
		node->primary_expression,
		level + 1,
		AST_PRINT_NO_INDENT_INITIAL | AST_PRINT_NO_TRAILING_NEWLINE);

	fprintf(stream, ",\n");

	FPRINT_AST_NODE_FINISH;
}
