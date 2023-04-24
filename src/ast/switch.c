// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * switch.c -- switch ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast/ast.h>
#include <jkcc/ast/switch.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/location.h>


ast_t *ast_switch_init(
	ast_t      *expression,
	ast_t      *statement,
	location_t *location_start,
	location_t *location_end)
{
	AST_INIT(ast_switch_t);

	node->expression = expression;
	node->statement  = statement;

	AST_NODE_LOCATION;

	AST_RETURN(AST_SWITCH);
}

void ast_switch_free(ast_t *ast)
{
	AST_FREE(ast_switch_t);

	AST_NODE_FREE(node->expression);
	AST_NODE_FREE(node->statement);

	free(node);
}

void fprint_ast_switch(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_switch_t);

	FPRINT_AST_MEMBER(ast_node_str[AST_EXPRESSION], node->expression);
	FPRINT_AST_MEMBER("statement", node->statement);

	FPRINT_AST_NODE_FINISH;
}
