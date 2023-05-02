// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * goto.c -- goto ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast/ast.h>
#include <jkcc/ast/goto.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/location.h>


ast_t *ast_goto_init(
	ast_t      *identifier,
	location_t *location_start,
	location_t *location_end)
{
	AST_INIT(ast_goto_t);

	node->identifier = identifier;

	node->label = NULL;

	AST_NODE_LOCATION;

	AST_RETURN(AST_GOTO);
}

void ast_goto_free(ast_t *ast)
{
	AST_FREE(ast_goto_t);

	AST_NODE_FREE(node->identifier);

	free(node);
}

ast_t *ast_goto_get_identifier(
	ast_t *ast_goto)
{
	return OFFSETOF_AST_NODE(ast_goto, ast_goto_t)->identifier;
}

void ast_goto_set_label(
	ast_t *ast_goto,
	ast_t *label)
{
	OFFSETOF_AST_NODE(ast_goto, ast_goto_t)->label = label;
}

void fprint_ast_goto(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_goto_t);

	FPRINT_AST_MEMBER(ast_node_str[AST_IDENTIFIER], node->identifier);

	FPRINT_AST_NODE_FINISH;
}
