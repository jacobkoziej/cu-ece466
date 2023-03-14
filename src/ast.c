// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * ast.c -- abstract syntax tree
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <jkcc/parser.h>
#include <jkcc/string.h>


ast_t *ast_identifier_init(string_t *identifier, location_t *location)
{
	ast_identifier_t *node = malloc(sizeof(*node));
	if (!node) return NULL;

	memcpy(&node->identifier, identifier, sizeof(*identifier));
	memcpy(&node->location, location, sizeof(*location));

	node->ast = AST_IDENTIFIER;

	return &node->ast;
}


static void fprint_location(
	FILE             *stream,
	const location_t *location,
	size_t            level,
	uint_fast8_t      flags)
{
	if (!(flags & AST_PRINT_NO_INDENT_INITIAL))
		INDENT(stream, level);

	fprintf(stream, "{\n");

	++level;

	INDENT(stream, level);
	fprintf(stream, "\"start\" : {\n");

	++level;

	INDENT(stream, level);
	fprintf(stream, "\"offset\" : %ld,\n", location->start.offset);
	INDENT(stream, level);
	fprintf(stream, "\"line\"   : %d,\n", location->start.line);
	INDENT(stream, level);
	fprintf(stream, "\"column\" : %d\n", location->start.column);

	--level;

	INDENT(stream, level);
	fprintf(stream, "},\n");

	INDENT(stream, level);
	fprintf(stream, "\"end\" : {\n");

	++level;

	INDENT(stream, level);
	fprintf(stream, "\"offset\" : %ld,\n", location->end.offset);
	INDENT(stream, level);
	fprintf(stream, "\"line\"   : %d,\n", location->end.line);
	INDENT(stream, level);
	fprintf(stream, "\"column\" : %d\n", location->end.column);

	--level;

	INDENT(stream, level);
	fprintf(stream, "}\n");

	--level;

	INDENT(stream, level);
	fprintf(stream, "}");

	if (!(flags & AST_PRINT_NO_TRAILING_NEWLINE))
		fprintf(stream, "\n");
}
