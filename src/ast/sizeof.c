// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * sizeof.c -- sizeof ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast/ast.h>
#include <jkcc/ast/sizeof.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/location.h>


ast_t *ast_sizeof_init(
	ast_t      *operand,
	location_t *location_start,
	location_t *location_end)
{
	AST_INIT(ast_sizeof_t);

	node->operand = operand;

	AST_NODE_LOCATION;

	AST_RETURN(AST_SIZEOF);
}

void ast_sizeof_free(ast_t *ast)
{
	AST_FREE(ast_sizeof_t);

	AST_NODE_FREE(node->operand);

	free(node);
}

ast_t *ast_sizeof_get_operand(ast_t *ast)
{
	return OFFSETOF_AST_NODE(ast, ast_sizeof_t)->operand;
}

void fprint_ast_sizeof(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_sizeof_t);

	FPRINT_AST_MEMBER("operand", node->operand);

	FPRINT_AST_NODE_FINISH;
}
