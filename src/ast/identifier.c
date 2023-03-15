// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * identifier.c -- identifier ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/identifier.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/lexer.h>
#include <jkcc/string.h>


ast_t *ast_identifier_init(identifier_t *identifier, location_t *location)
{
	AST_INIT(ast_identifier_t);

	node->identifier = *identifier;
	node->location   = *location;

	AST_RETURN(AST_IDENTIFIER);
}

void ast_identifier_free(ast_t *ast)
{
	AST_FREE(ast_identifier_t);

	string_free(&node->identifier.IDENTIFIER);
	string_free(&node->identifier.text);
	free(node);
}

void fprint_ast_identifier(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_identifier_t);

	INDENT(stream, level);
	fprintf(
		stream,
		"\"identifier\" : \"%s\",\n",
		node->identifier.text.head);

	FPRINT_AST_NODE_FINISH;
}
