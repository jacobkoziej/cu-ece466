// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * character_constant.c -- character_constant ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/character_constant.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/lexer.h>
#include <jkcc/string.h>


ast_t *ast_character_constant_init(
	character_constant_t *character_constant,
	location_t           *location)
{
	AST_INIT(ast_character_constant_t);

	node->character_constant = *character_constant;
	node->location           = *location;

	AST_RETURN(AST_CHARACTER_CONSTANT);
}

void ast_character_constant_free(ast_t *ast)
{
	AST_FREE(ast_character_constant_t);

	string_free(&node->character_constant.text);
	free(node);
}

void fprint_ast_character_constant(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_character_constant_t);

	INDENT(stream, level);
	fprintf(stream, "\"type\"  : \"");

	const char *type;
	switch (node->character_constant.type) {
		case UNSIGNED_CHAR:
			type = "unsigned char";
			break;

		case WCHAR_T:
			type = "wchar_t";
			break;

		case CHAR16_T:
			type = "char16_t";
			break;

		case CHAR32_T:
			type = "char32_t";
			break;

		default:
			type = "(unknown)";
	}

	fprintf(stream, "%s\",\n", type);

	INDENT(stream, level);
	fprintf(
		stream,
		"\"value\" : \"%s\",\n",
		node->character_constant.text.head);

	FPRINT_AST_NODE_FINISH;
}
