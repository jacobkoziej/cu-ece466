// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * while.c -- while ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast/ast.h>
#include <jkcc/ast/while.h>
#include <jkcc/private/ast.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/location.h>


ast_t *ast_while_init(
	ast_t      *expression,
	ast_t      *statement,
	bool        do_while,
	location_t *location_start,
	location_t *location_end)
{
	AST_INIT(ast_while_t);

	node->expression  = expression;
	node->statement   = statement;
	node->do_while    = do_while;

	AST_NODE_LOCATION;

	AST_RETURN(AST_WHILE);
}

void ast_while_free(ast_t *ast)
{
	AST_FREE(ast_while_t);

	AST_NODE_FREE(node->expression);
	AST_NODE_FREE(node->statement);

	free(node);
}

ast_t *ast_while_get_expression(ast_t *ast)
{
	return OFFSETOF_AST_NODE(ast, ast_while_t)->expression;
}

void fprint_ast_while(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_while_t);

	FPRINT_AST_MEMBER(ast_node_str[AST_EXPRESSION], node->expression);
	FPRINT_AST_MEMBER("statement", node->statement);
	FPRINT_AST_BOOL("do-while", node->do_while);

	FPRINT_AST_NODE_FINISH;
}
