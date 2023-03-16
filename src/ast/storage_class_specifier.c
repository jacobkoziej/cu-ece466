// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * storage_class_specifier.c -- storage_class_specifier ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/storage_class_specifier.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/lexer.h>
#include <jkcc/string.h>

#include "y.tab.h"


ast_t *ast_storage_class_specifier_init(int type, location_t *location)
{
	AST_INIT(ast_storage_class_specifier_t);

	node->type     =  type;
	node->location = *location;

	AST_RETURN(AST_STORAGE_CLASS_SPECIFIER);
}

void ast_storage_class_specifier_free(ast_t *ast)
{
	AST_FREE(ast_storage_class_specifier_t);

	free(node);
}

void fprint_ast_storage_class_specifier(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_storage_class_specifier_t);

	const char *type;
	switch (node->type) {
		case KEYWORD_TYPEDEF:
			type = "typedef";
			break;

		case KEYWORD_EXTERN:
			type = "extern";
			break;

		case KEYWORD_STATIC:
			type = "static";
			break;

		case KEYWORD__THREAD_LOCAL:
			type = "_Thread_local";
			break;

		case KEYWORD_AUTO:
			type = "auto";
			break;

		case KEYWORD_REGISTER:
			type = "register";
			break;

		default:
			type = "(unknown)";
			break;
	}

	INDENT(stream, level);
	fprintf(stream, "\"type\" : \"%s\",\n", type);

	FPRINT_AST_NODE_FINISH;
}
