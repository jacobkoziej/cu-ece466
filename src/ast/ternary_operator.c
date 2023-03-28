// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * ternary_operator.c -- ternary_operator ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/ternary_operator.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/lexer.h>


ast_t *ast_ternary_operator_init(
	ast_t        *condition,
	ast_t        *lhs,
	ast_t        *rhs,
	location_t   *location_start,
	location_t   *location_end)
{
	AST_INIT(ast_ternary_operator_t);

	node->condition = condition;
	node->lhs       = lhs;
	node->rhs       = rhs;

	node->location.file  = location_start->file;
	node->location.start = location_start->start;

	node->location.end = (location_end)
		? location_end->end
		: location_start->end;

	AST_RETURN(AST_TERNARY_OPERATOR);
}

void ast_ternary_operator_free(ast_t *ast)
{
	AST_FREE(ast_ternary_operator_t);

	AST_NODE_FREE(node->condition);
	AST_NODE_FREE(node->lhs);
	AST_NODE_FREE(node->rhs);

	free(node);
}

void fprint_ast_ternary_operator(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_ternary_operator_t);

	FPRINT_AST_MEMBER("condition", node->condition);
	FPRINT_AST_MEMBER("lhs", node->lhs);
	FPRINT_AST_MEMBER("rhs", node->rhs);

	FPRINT_AST_NODE_FINISH;
}
