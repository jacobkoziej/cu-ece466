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
		&node->assignment_expression,
		&assignment_expression))
		goto error;

	node->location.end = location->end;

	return expression;

error:
	return NULL;
}

ast_t *ast_expression_init(
	ast_t      *assignment_expression,
	location_t *location)
{
	AST_INIT(ast_expression_t);

	if (vector_init(
		&node->assignment_expression,
		sizeof(assignment_expression),
		0))
		goto error_init;

	if (vector_append(
		&node->assignment_expression,
		&assignment_expression))
		goto error_append;

	node->location = *location;

	AST_RETURN(AST_EXPRESSION);

error_append:
	vector_free(&node->assignment_expression);

error_init:
	return NULL;
}

void ast_expression_free(ast_t *ast)
{
	AST_FREE(ast_expression_t);

	ast_t **assignment_expression = node->assignment_expression.buf;

	for (size_t i = 0; i < node->assignment_expression.use; i++)
		AST_NODE_FREE(assignment_expression[i]);

	vector_free(&node->assignment_expression);

	free(node);
}

void fprint_ast_expression(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_expression_t);

	INDENT(stream, level);
	fprintf(
		stream,
		"\"%s\" : [\n",
		ast_node_str[AST_ASSIGNMENT_EXPRESSION]);

	++level;

	ast_t **assignment_expression = node->assignment_expression.buf;

	size_t pos;
	for (pos = 0; pos < node->assignment_expression.use - 1; pos++) {
		FPRINT_AST_NODE(
			stream,
			assignment_expression[pos],
			level,
			AST_PRINT_NO_TRAILING_NEWLINE);

		fprintf(stream, ",\n");
	}

	FPRINT_AST_NODE(stream, assignment_expression[pos], level, 0);

	--level;

	INDENT(stream, level);
	fprintf(stream, "],\n");

	FPRINT_AST_NODE_FINISH;
}
