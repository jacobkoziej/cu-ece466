// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * type_qualifier.c -- type_qualifier ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/type_qualifier.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/lexer.h>
#include <jkcc/string.h>

#include "y.tab.h"


ast_t *ast_type_qualifier_init(int type, location_t *location)
{
	AST_INIT(ast_type_qualifier_t);

	node->type     =  type;
	node->location = *location;

	AST_RETURN(AST_TYPE_QUALIFIER);
}

void ast_type_qualifier_free(ast_t *ast)
{
	AST_FREE(ast_type_qualifier_t);

	free(node);
}

void fprint_ast_type_qualifier(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_type_qualifier_t);

	const char *type;
	switch (node->type) {
		case KEYWORD_CONST:
			type = "const";
			break;

		case KEYWORD_RESTRICT:
			type = "restrict";
			break;

		case KEYWORD_VOLATILE:
			type = "volatile";
			break;

		case KEYWORD__ATOMIC:
			type = "_Atomic";
			break;

		default:
			type = "(unknown)";
			break;
	}

	INDENT(stream, level);
	fprintf(stream, "\"type\" : \"%s\",\n", type);

	FPRINT_AST_NODE_FINISH;
}
