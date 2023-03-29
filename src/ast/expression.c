// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * expression.c -- expression ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/expression.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/lexer.h>
#include <jkcc/vector.h>


ast_t *ast_expression_append(
	ast_t      *expression,
	ast_t      *assignment_expression,
	location_t *location)
{
	ast_expression_t *node = OFFSETOF_AST_NODE(
		expression,
		ast_expression_t);

	if (vector_append(
		&node->expression,
		&assignment_expression))
		goto error;

	node->location.end = location->end;

	return expression;

error:
	return NULL;
}

ast_t *ast_expression_init(
	ast_t      *expression,
	ast_t      *assignment_expression,
	location_t *location_start,
	location_t *location_end)
{
	AST_INIT(ast_expression_t);

	if (vector_init(&node->expression, sizeof(expression), 0))
		goto error_init;

	if (vector_append(&node->expression, &expression))
		goto error_append;

	if (vector_append(&node->expression, &assignment_expression))
		goto error_append;

	AST_NODE_LOCATION;

	AST_RETURN(AST_EXPRESSION);

error_append:
	vector_free(&node->expression);

error_init:
	return NULL;
}

void ast_expression_free(ast_t *ast)
{
	AST_FREE(ast_expression_t);

	ast_t **expression = node->expression.buf;

	for (size_t i = 0; i < node->expression.use; i++)
		AST_NODE_FREE(expression[i]);

	vector_free(&node->expression);

	free(node);
}

void fprint_ast_expression(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_expression_t);

	FPRINT_AST_LIST(ast_node_str[AST_EXPRESSION], node->expression);

	FPRINT_AST_NODE_FINISH;
}
