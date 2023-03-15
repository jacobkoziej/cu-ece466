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

#include <jkcc/lexer.h>
#include <jkcc/list.h>
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


static void fprint_ast_identifier(
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

	INDENT(stream, level);
	fprintf(stream, "\"location\" : ");
	fprint_location(
		stream,
		&node->location,
		level,
		AST_PRINT_NO_INDENT_INITIAL);

	FPRINT_AST_FINISH;
}

static void fprint_file(
	FILE             *stream,
	const file_t     *file,
	size_t            level,
	uint_fast8_t      flags)
{
	FPRINT_AST_BEGIN;

	INDENT(stream, level);
	fprintf(stream, "\"path\" : [\n");

	++level;

	const list_t *tail = &file->list;
	const file_t *cur  = file;

	while (tail->prev) {
		cur = OFFSETOF_LIST(tail, file_t, list);

		INDENT(stream, level);
		fprintf(stream, "\"%s\",\n", cur->path);

		tail = tail->prev;
	}

	INDENT(stream, level);
	fprintf(stream, "\"%s\"\n", cur->path);

	--level;

	INDENT(stream, level);
	fprintf(stream, "],\n");

	INDENT(stream, level);
	fprintf(stream, "\"refs\" : %lu\n", file->refs);

	FPRINT_AST_FINISH;
}

static void fprint_location(
	FILE             *stream,
	const location_t *location,
	size_t            level,
	uint_fast8_t      flags)
{
	FPRINT_AST_BEGIN;

	INDENT(stream, level);
	fprintf(stream, "\"file\" : ");
	fprint_file(
		stream,
		location->file,
		level,
		AST_PRINT_NO_INDENT_INITIAL | AST_PRINT_NO_TRAILING_NEWLINE);
	fprintf(stream, ",\n");

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

	FPRINT_AST_FINISH;
}
