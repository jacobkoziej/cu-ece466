// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * generic_selection.c -- generic_selection ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/generic_selection.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/lexer.h>


ast_t *ast_generic_selection_init(
	ast_t      *expression,
	ast_t      *generic_association_list,
	location_t *location_start,
	location_t *location_end)
{
	AST_INIT(ast_generic_selection_t);

	node->expression               = expression;
	node->generic_association_list = generic_association_list;

	AST_NODE_LOCATION;

	AST_RETURN(AST_GENERIC_SELECTION);
}

void ast_generic_selection_free(ast_t *ast)
{
	AST_FREE(ast_generic_selection_t);

	AST_NODE_FREE(node->expression);
	AST_NODE_FREE(node->generic_association_list);

	free(node);
}

void fprint_ast_generic_selection(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_generic_selection_t);

	FPRINT_AST_MEMBER("expression", node->expression);
	FPRINT_AST_MEMBER(
		ast_node_str[AST_GENERIC_ASSOCIATION_LIST],
		node->generic_association_list);

	FPRINT_AST_NODE_FINISH;
}
