// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * function_specifier.c -- function_specifier ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/function_specifier.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/lexer.h>

#include "y.tab.h"


ast_t *ast_function_specifier_init(
	uint_fast8_t  specifier,
	location_t   *location)
{
	AST_INIT(ast_function_specifier_t);

	node->specifier =  specifier;
	node->location  = *location;

	AST_RETURN(AST_FUNCTION_SPECIFIER);
}

void ast_function_specifier_free(ast_t *ast)
{
	AST_FREE(ast_function_specifier_t);

	free(node);
}

void fprint_ast_function_specifier(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_function_specifier_t);

	const char *specifier;
	switch (node->specifier) {
		case AST_FUNCTION_SPECIFIER_INLINE:
			specifier = "inline";
			break;

		case AST_FUNCTION_SPECIFIER__NORETURN:
			specifier = "_Noreturn";
			break;

		default:
			specifier = "(unknown)";
			break;
	}

	FPRINT_AST_FIELD("specifier", specifier);

	FPRINT_AST_NODE_FINISH;
}
