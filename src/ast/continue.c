// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * continue.c -- continue ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast/ast.h>
#include <jkcc/ast/continue.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/location.h>


ast_t *ast_continue_init(
	location_t *location)
{
	AST_INIT(ast_continue_t);

	node->location = *location;

	AST_RETURN(AST_CONTINUE);
}

void ast_continue_free(ast_t *ast)
{
	AST_FREE(ast_continue_t);

	free(node);
}

void fprint_ast_continue(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_continue_t);

	FPRINT_AST_NODE_FINISH;
}
