// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * generic_association.c -- generic_association ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast/ast.h>
#include <jkcc/ast/generic_association.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/location.h>


ast_t *ast_generic_association_init(
	ast_t      *type,
	ast_t      *expression,
	location_t *location_start,
	location_t *location_end)
{
	AST_INIT(ast_generic_association_t);

	node->type       = type;
	node->expression = expression;

	AST_NODE_LOCATION;

	AST_RETURN(AST_GENERIC_ASSOCIATION);
}

void ast_generic_association_free(ast_t *ast)
{
	AST_FREE(ast_generic_association_t);

	AST_NODE_FREE(node->type);
	AST_NODE_FREE(node->expression);

	free(node);
}

void fprint_ast_generic_association(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_generic_association_t);

	FPRINT_AST_MEMBER("type", node->type);
	FPRINT_AST_MEMBER(ast_node_str[AST_EXPRESSION], node->expression);

	FPRINT_AST_NODE_FINISH;
}
