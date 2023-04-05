// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * static_assert.c -- static_assert ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/static_assert.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/location.h>


ast_t *ast_static_assert_init(
	ast_t      *constant_expression,
	ast_t      *string_literal,
	location_t *location_start,
	location_t *location_end)
{
	AST_INIT(ast_static_assert_t);

	node->constant_expression = constant_expression;
	node->string_literal      = string_literal;

	AST_NODE_LOCATION;

	AST_RETURN(AST_STATIC_ASSERT);
}

void ast_static_assert_free(ast_t *ast)
{
	AST_FREE(ast_static_assert_t);

	AST_NODE_FREE(node->constant_expression);
	AST_NODE_FREE(node->string_literal);

	free(node);
}

void fprint_ast_static_assert(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_static_assert_t);

	FPRINT_AST_MEMBER("constant-expression", node->constant_expression);
	FPRINT_AST_MEMBER(
		ast_node_str[AST_STRING_LITERAL],
		node->string_literal);

	FPRINT_AST_NODE_FINISH;
}
