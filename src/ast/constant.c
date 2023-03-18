// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * constant.c -- constant ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/constant.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/lexer.h>


ast_t *ast_constant_init(
	ast_t      *constant,
	location_t *location)
{
	AST_INIT(ast_constant_t);

	node->constant =  constant;
	node->location = *location;

	AST_RETURN(AST_CONSTANT);
}

void ast_constant_free(ast_t *ast)
{
	AST_FREE(ast_constant_t);

	AST_NODE_FREE(node->constant);

	free(node);
}

void fprint_ast_constant(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_constant_t);

	const char *type;
	switch (*node->constant) {
		case AST_INTEGER_CONSTANT:
			type = "integer-constant";
			break;

		case AST_FLOATING_CONSTANT:
			type = "floating-constant";
			break;

		/* TODO: add support for enums
		case AST_ENUMERATION_CONSTANT:
			type = "enumeration-constant";
			break;
		*/

		case AST_CHARACTER_CONSTANT:
			type = "character-constant";
			break;

		default:
			type = "(unknown)";
			break;
	}

	INDENT(stream, level);
	fprintf(stream, "\"%s\" : ", type);

	FPRINT_AST_NODE(
		stream,
		node->constant,
		level + 1,
		AST_PRINT_NO_INDENT_INITIAL | AST_PRINT_NO_TRAILING_NEWLINE);

	fprintf(stream, ",\n");

	FPRINT_AST_NODE_FINISH;
}
