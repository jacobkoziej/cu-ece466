// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * type_specifier.c -- type_specifier ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/type_specifier.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/lexer.h>

#include "y.tab.h"


ast_t *ast_type_specifier_init(
	int         keyword_type,
	ast_t      *semantic_type,
	location_t *location)
{
	AST_INIT(ast_type_specifier_t);

	node->keyword_type  =  keyword_type,
	node->semantic_type =  semantic_type,
	node->location      = *location;

	AST_RETURN(AST_TYPE_SPECIFIER);
}

void ast_type_specifier_free(ast_t *ast)
{
	AST_FREE(ast_type_specifier_t);

	if (node->semantic_type) AST_NODE_FREE(node->semantic_type);
	free(node);
}

void fprint_ast_type_specifier(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_type_specifier_t);

	INDENT(stream, level);
	fprintf(stream, "\"type\" : ");

	if (node->semantic_type) {
		fprintf(stream, "\"(semantic)\",\n");
	} else {
		const char *type;
		switch (node->keyword_type) {
			case KEYWORD_VOID:
				type = "void";
				break;

			case KEYWORD_CHAR:
				type = "char";
				break;

			case KEYWORD_SHORT:
				type = "short";
				break;

			case KEYWORD_INT:
				type = "int";
				break;

			case KEYWORD_LONG:
				type = "long";
				break;

			case KEYWORD_FLOAT:
				type = "float";
				break;

			case KEYWORD_DOUBLE:
				type = "double";
				break;

			case KEYWORD_UNSIGNED:
				type = "unsigned";
				break;

			case KEYWORD__BOOL:
				type = "_Bool";
				break;

			case KEYWORD__COMPLEX:
				type = "_Complex";
				break;

			default:
				type = "(unknown)";
				break;
		}

		fprintf(stream, "\"%s\",\n", type);
	}

	FPRINT_AST_NODE_FINISH;
}
