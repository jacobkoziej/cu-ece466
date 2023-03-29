// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * string_literal.c -- string_literal ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/string_literal.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/lexer.h>
#include <jkcc/string.h>


ast_t *ast_string_literal_init(
	string_literal_t *string_literal,
	location_t       *location)
{
	AST_INIT(ast_string_literal_t);

	node->string_literal = *string_literal;
	node->location       = *location;

	AST_RETURN(AST_STRING_LITERAL);
}

void ast_string_literal_free(ast_t *ast)
{
	AST_FREE(ast_string_literal_t);

	string_free(&node->string_literal.string);
	string_free(&node->string_literal.text);
	free(node);
}

void fprint_ast_string_literal(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_string_literal_t);

	const char *encoding;
	const char *prefix;
	switch (node->string_literal.encoding) {
		case STRING_CHAR:
			encoding = "char*";
			prefix   = "";
			break;

		case STRING_UTF_8:
			encoding = "char*";
			prefix   = "u8";
			break;

		case STRING_CHAR16_T:
			encoding = "char16_t*";
			prefix   = "u";
			break;

		case STRING_CHAR32_T:
			encoding = "char32_t*";
			prefix   = "U";
			break;

		case STRING_WCHAR_T:
			encoding = "wchar_t*";
			prefix   = "L";
			break;

		default:
			encoding = "(unknown)";
	}

	FPRINT_AST_FIELD("encoding", encoding);

	// escape quotes
	const char *head = node->string_literal.text.head;
	node->string_literal.text.tail[-1] = '\0';

	// strip prefix
	if (*head != '"') {
		head += 2;

		// u8
		if (*head == '"') ++head;
	} else ++head;

	INDENT(stream, level);
	fprintf(
		stream,
		"\"value\"    : \"%s\\\"%s\\\"\",\n",
		prefix,
		head);

	// restore quote
	node->string_literal.text.tail[-1] = '"';

	FPRINT_AST_NODE_FINISH;
}
