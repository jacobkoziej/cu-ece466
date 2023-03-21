// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * postfix_expression.c -- postfix_expression ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/postfix_expression.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/lexer.h>


ast_t *ast_postfix_expression_init(
	ast_t        *postfix_expression,
	ast_t        *expression,
	ast_t        *argument_expression_list,
	ast_t        *identifier,
	ast_t        *type_name,
	ast_t        *initializer_list,
	uint_fast8_t  flags,
	location_t   *location_start,
	location_t   *location_end)
{
	AST_INIT(ast_postfix_expression_t);

	node->postfix_expression       = postfix_expression;
	node->expression               = expression;
	node->argument_expression_list = argument_expression_list;
	node->identifier               = identifier;
	node->type_name                = type_name;
	node->initializer_list         = initializer_list;

	node->flags = flags;

	node->location.file  = location_start->file;
	node->location.start = location_start->start;

	node->location.end = (location_end)
		? location_end->end
		: location_start->end;

	AST_RETURN(AST_POSTFIX_EXPRESSION);
}

void ast_postfix_expression_free(ast_t *ast)
{
	AST_FREE(ast_postfix_expression_t);

	AST_NODE_FREE(node->postfix_expression);
	AST_NODE_FREE(node->expression);
	AST_NODE_FREE(node->argument_expression_list);
	AST_NODE_FREE(node->identifier);
	AST_NODE_FREE(node->type_name);
	AST_NODE_FREE(node->initializer_list);

	free(node);
}

void fprint_ast_postfix_expression(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_postfix_expression_t);

	FPRINT_AST_MEMBER(
		ast_node_str[AST_POSTFIX_EXPRESSION],
		node->postfix_expression);
	FPRINT_AST_MEMBER(
		ast_node_str[AST_EXPRESSION],
		node->expression);
	/* TODO: AST_ARGUMENT_EXPRESSION_LIST
	FPRINT_AST_MEMBER(
		ast_node_str[AST_ARGUMENT_EXPRESSION_LIST],
		node->argument_expression_list);
	*/
	FPRINT_AST_MEMBER(
		ast_node_str[AST_IDENTIFIER],
		node->identifier);
	/* TODO: AST_INITIALIZER_LIST
	FPRINT_AST_MEMBER(
		ast_node_str[AST_INITIALIZER_LIST],
		node->initializer_list);
	*/

	INDENT(stream, level);
	fprintf(
		stream,
		"\"dereference\" : %s,\n",
		(node->flags & POSTFIX_EXPRESSION_DEREFERENCE)
		? "true"
		: "false");

	INDENT(stream, level);
	fprintf(
		stream,
		"\"parenthesis\" : %s,\n",
		(node->flags & POSTFIX_EXPRESSION_PARENTHESIS)
		? "true"
		: "false");

	INDENT(stream, level);
	fprintf(
		stream,
		"\"increment\"   : %s,\n",
		(node->flags & POSTFIX_EXPRESSION_INCREMENT)
		? "true"
		: "false");

	INDENT(stream, level);
	fprintf(
		stream,
		"\"decrement\"   : %s,\n",
		(node->flags & POSTFIX_EXPRESSION_DECREMENT)
		? "true"
		: "false");

	FPRINT_AST_NODE_FINISH;
}
