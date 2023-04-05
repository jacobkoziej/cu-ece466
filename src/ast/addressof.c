// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * addressof.c -- addressof ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/addressof.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/location.h>


ast_t *ast_addressof_init(
	ast_t      *operand,
	location_t *location_start,
	location_t *location_end)
{
	AST_INIT(ast_addressof_t);

	node->operand = operand;

	AST_NODE_LOCATION;

	AST_RETURN(AST_ADDRESSOF);
}

void ast_addressof_free(ast_t *ast)
{
	AST_FREE(ast_addressof_t);

	AST_NODE_FREE(node->operand);

	free(node);
}

void fprint_ast_addressof(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_addressof_t);

	FPRINT_AST_MEMBER("operand", node->operand);

	FPRINT_AST_NODE_FINISH;
}
