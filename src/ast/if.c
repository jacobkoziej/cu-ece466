// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * if.c -- if ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast/ast.h>
#include <jkcc/ast/if.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/location.h>


ast_t *ast_if_init(
	ast_t      *expression,
	ast_t      *true_statement,
	ast_t      *false_statement,
	location_t *location_start,
	location_t *location_end)
{
	AST_INIT(ast_if_t);

	node->expression      = expression;
	node->true_statement  = true_statement;
	node->false_statement = false_statement;

	AST_NODE_LOCATION;

	AST_RETURN(AST_IF);
}

void ast_if_free(ast_t *ast)
{
	AST_FREE(ast_if_t);

	AST_NODE_FREE(node->expression);
	AST_NODE_FREE(node->true_statement);
	AST_NODE_FREE(node->false_statement);

	free(node);
}

ast_t *ast_if_get_true_statement(ast_t *ast)
{
	return OFFSETOF_AST_NODE(ast, ast_if_t)->true_statement;
}

void fprint_ast_if(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_if_t);

	FPRINT_AST_MEMBER(ast_node_str[AST_EXPRESSION], node->expression);
	FPRINT_AST_MEMBER("true-statement", node->true_statement);
	FPRINT_AST_MEMBER("false-statement", node->false_statement);

	FPRINT_AST_NODE_FINISH;
}
