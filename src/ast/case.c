// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * case.c -- case ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast/ast.h>
#include <jkcc/ast/case.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/location.h>


ast_t *ast_case_init(
	ast_t      *constant_expression,
	ast_t      *statement,
	location_t *location_start,
	location_t *location_end)
{
	AST_INIT(ast_case_t);

	node->constant_expression = constant_expression;
	node->statement           = statement;

	AST_NODE_LOCATION;

	AST_RETURN(AST_CASE);
}

void ast_case_free(ast_t *ast)
{
	AST_FREE(ast_case_t);

	AST_NODE_FREE(node->constant_expression);
	AST_NODE_FREE(node->statement);

	free(node);
}

void fprint_ast_case(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_case_t);

	FPRINT_AST_MEMBER("constant-expression", node->constant_expression);
	FPRINT_AST_MEMBER("statement", node->statement);

	FPRINT_AST_NODE_FINISH;
}
