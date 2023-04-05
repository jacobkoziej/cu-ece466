// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * integer_constant.c -- integer_constant ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/integer_constant.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/constant.h>
#include <jkcc/location.h>
#include <jkcc/string.h>


ast_t *ast_integer_constant_init(
	integer_constant_t *integer_constant,
	location_t         *location)
{
	AST_INIT(ast_integer_constant_t);

	node->integer_constant = *integer_constant;
	node->location         = *location;

	AST_RETURN(AST_INTEGER_CONSTANT);
}

void ast_integer_constant_free(ast_t *ast)
{
	AST_FREE(ast_integer_constant_t);

	string_free(&node->integer_constant.text);
	free(node);
}

void fprint_ast_integer_constant(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_integer_constant_t);

	const char *type;
	switch (node->integer_constant.type) {
		case INT:
			type = "int";
			break;

		case UNSIGNED_INT:
			type = "unsigned int";
			break;

		case LONG_INT:
			type = "long int";
			break;

		case UNSIGNED_LONG_INT:
			type = "unsigned long int";
			break;

		case LONG_LONG_INT:
			type = "long long int";
			break;

		case UNSIGNED_LONG_LONG_INT:
			type = "unsigned long long int";
			break;

		default:
			type = "(unknown)";
	}

	FPRINT_AST_FIELD("type", type);
	FPRINT_AST_FIELD("value", node->integer_constant.text.head);

	FPRINT_AST_NODE_FINISH;
}
