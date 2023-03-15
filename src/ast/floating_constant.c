// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * floating_constant.c -- floating_constant ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/floating_constant.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/lexer.h>
#include <jkcc/string.h>


ast_t *ast_floating_constant_init(
	floating_constant_t *floating_constant,
	location_t          *location)
{
	AST_INIT(ast_floating_constant_t);

	node->floating_constant = *floating_constant;
	node->location          = *location;

	AST_RETURN(AST_FLOATING_CONSTANT);
}

void ast_floating_constant_free(ast_t *ast)
{
	AST_FREE(ast_floating_constant_t);

	string_free(&node->floating_constant.text);
	free(node);
}

void fprint_ast_floating_constant(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_floating_constant_t);

	INDENT(stream, level);
	fprintf(stream, "\"type\"  : \"");

	const char *type;
	switch (node->floating_constant.type) {
		case FLOAT:
			type = "float";
			break;

		case DOUBLE:
			type = "double";
			break;

		case LONG_DOUBLE:
			type = "long double";
			break;

		default:
			type = "(unknown)";
	}

	fprintf(stream, "%s\",\n", type);

	INDENT(stream, level);
	fprintf(
		stream,
		"\"value\" : \"%s\",\n",
		node->floating_constant.text.head);

	FPRINT_AST_NODE_FINISH;
}
