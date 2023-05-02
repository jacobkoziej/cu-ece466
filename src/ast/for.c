// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * for.c -- for ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast/ast.h>
#include <jkcc/ast/for.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/location.h>


ast_t *ast_for_init(
	ast_t      *initializer,
	ast_t      *condition,
	ast_t      *iteration,
	ast_t      *statement,
	location_t *location_start,
	location_t *location_end)
{
	AST_INIT(ast_for_t);

	node->initializer = initializer;
	node->condition   = condition;
	node->iteration   = iteration;
	node->statement   = statement;

	AST_NODE_LOCATION;

	AST_RETURN(AST_FOR);
}

void ast_for_free(ast_t *ast)
{
	AST_FREE(ast_for_t);

	AST_NODE_FREE(node->initializer);
	AST_NODE_FREE(node->condition);
	AST_NODE_FREE(node->iteration);
	AST_NODE_FREE(node->statement);

	free(node);
}

void fprint_ast_for(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_for_t);

	FPRINT_AST_MEMBER("initializer", node->initializer);
	FPRINT_AST_MEMBER("condition", node->condition);
	FPRINT_AST_MEMBER("iteration", node->iteration);
	FPRINT_AST_MEMBER("statement", node->statement);

	FPRINT_AST_NODE_FINISH;
}
