// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * ast.c -- abstract syntax tree
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <jkcc/parser.h>
#include <jkcc/string.h>


void (*ast_node_free[AST_NODES_TOTAL])(ast_t *ast) = {
	ast_identifier_free,
};

void (*fprint_ast_node[AST_NODES_TOTAL])(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags) = {
	fprint_ast_identifier,
};


ast_t *ast_identifier_init(string_t *identifier, location_t *location)
{
	ast_identifier_t *node = malloc(sizeof(*node));
	if (!node) return NULL;

	memcpy(&node->identifier, identifier, sizeof(*identifier));
	memcpy(&node->location, location, sizeof(*location));

	node->ast = AST_IDENTIFIER;

	return &node->ast;
}

void ast_identifier_free(ast_t *ast)
{
	ast_identifier_t *node = OFFSETOF_AST_NODE(ast, ast_identifier_t);

	string_free(&node->identifier);
	free(node);
}


static void fprint_ast_identifier(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	ast_identifier_t *node = OFFSETOF_AST_NODE(ast, ast_identifier_t);

	if (!(flags & AST_PRINT_NO_INDENT_INITIAL))
		INDENT(stream, level);

	fprintf(stream, "{\n");

	++level;

	INDENT(stream, level);
	fprintf(stream, "\"identifier\" : \"%s\",\n", node->identifier.head);

	INDENT(stream, level);
	fprintf(stream, "\"location\" : ");
	fprint_location(
		stream,
		&node->location,
		level,
		AST_PRINT_NO_INDENT_INITIAL);

	--level;

	INDENT(stream, level);
	fprintf(stream, "}");

	if (!(flags & AST_PRINT_NO_TRAILING_NEWLINE))
		fprintf(stream, "\n");
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
