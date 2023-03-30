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

#include "y.tab.h"


ast_t *ast_type_qualifier_init(
	uint_fast8_t  qualifier,
	location_t   *location)
{
	AST_INIT(ast_type_qualifier_t);

	node->qualifier =  qualifier;
	node->location  = *location;

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

	const char *qualifier;
	switch (node->qualifier) {
		case AST_TYPE_QUALIFIER_CONST:
			qualifier = "const";
			break;

		case AST_TYPE_QUALIFIER_RESTRICT:
			qualifier = "restrict";
			break;

		case AST_TYPE_QUALIFIER_VOLATILE:
			qualifier = "volatile";
			break;

		case AST_TYPE_QUALIFIER__ATOMIC:
			qualifier = "_Atomic";
			break;

		default:
			qualifier = "(unknown)";
			break;
	}

	FPRINT_AST_FIELD("qualifier", qualifier);

	FPRINT_AST_NODE_FINISH;
}
