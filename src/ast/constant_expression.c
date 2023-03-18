// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * constant_expression.c -- constant_expression ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/constant_expression.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/lexer.h>


ast_t *ast_constant_expression_init(
	ast_t      *conditional_expression,
	location_t *location)
{
	AST_INIT(ast_constant_expression_t);

	node->conditional_expression =  conditional_expression;
	node->location               = *location;

	AST_RETURN(AST_CONSTANT_EXPRESSION);
}

void ast_constant_expression_free(ast_t *ast)
{
	AST_FREE(ast_constant_expression_t);

	AST_NODE_FREE(node->conditional_expression);

	free(node);
}

void fprint_ast_constant_expression(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_constant_expression_t);

	INDENT(stream, level);
	fprintf(stream, "\"conditional-expression\" : ");

	FPRINT_AST_NODE(
		stream,
		node->conditional_expression,
		level + 1,
		AST_PRINT_NO_INDENT_INITIAL |
		AST_PRINT_NO_TRAILING_NEWLINE);

	fprintf(stream, ",\n");

	FPRINT_AST_NODE_FINISH;
}
