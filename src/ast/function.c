// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * function.c -- function ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast/ast.h>
#include <jkcc/ast/function.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/location.h>


ast_t *ast_function_init(
	ast_t        *identifier,
	ast_t        *parameter_list,
	ast_t        *identifier_list,
	bool          variadic,
	location_t   *location_start,
	location_t   *location_end)
{
	AST_INIT(ast_function_t);

	node->identifier      = identifier;
	node->parameter_list  = parameter_list;
	node->identifier_list = identifier_list;
	node->variadic        = variadic;

	node->return_type      = NULL;
	node->declaration_list = NULL;
	node->body             = NULL;

	AST_NODE_LOCATION;

	AST_RETURN(AST_FUNCTION);
}

void ast_function_free(ast_t *ast)
{
	AST_FREE(ast_function_t);

	// ast_function_t only references ast_type_t,
	// we want to avoid a double free here
	if (node->return_type)
		switch (*node->return_type) {
			case AST_ARRAY:
			case AST_POINTER:
				AST_NODE_FREE(node->return_type);
				break;

			default:
				break;
		}

	// ast_function_t only references ast_identifier_t
	// when it is a definition
	if (node->body) AST_NODE_FREE(node->identifier);

	AST_NODE_FREE(node->parameter_list);
	AST_NODE_FREE(node->identifier_list);
	AST_NODE_FREE(node->declaration_list);
	AST_NODE_FREE(node->body);

	free(node);
}

ast_t *ast_function_get_body(
	ast_t *function)
{
	return OFFSETOF_AST_NODE(function, ast_function_t)->body;
}

ast_t *ast_function_get_declaration_list(
	ast_t *function)
{
	return OFFSETOF_AST_NODE(function, ast_function_t)->declaration_list;
}

void ast_function_set_body(
	ast_t *function,
	ast_t *body)
{
	ast_function_t *ast_function = OFFSETOF_AST_NODE(
		function,
		ast_function_t);

	ast_function->body = body;
}

void ast_function_set_declaration_list(
	ast_t *function,
	ast_t *declaration_list)
{
	ast_function_t *ast_function = OFFSETOF_AST_NODE(
		function,
		ast_function_t);

	ast_function->declaration_list = declaration_list;
}

void ast_function_set_return_type(
	ast_t *function,
	ast_t *return_type)
{
	ast_function_t *ast_function = OFFSETOF_AST_NODE(
		function,
		ast_function_t);

	ast_function->return_type = return_type;
}

void fprint_ast_function(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_function_t);

	FPRINT_AST_MEMBER(ast_node_str[AST_IDENTIFIER], node->identifier);
	FPRINT_AST_MEMBER("return-type", node->return_type);
	FPRINT_AST_MEMBER("parameter-list", node->parameter_list);
	FPRINT_AST_MEMBER("identifier-list", node->identifier_list);
	FPRINT_AST_MEMBER("declaration-list", node->declaration_list);
	FPRINT_AST_MEMBER("body", node->body);
	FPRINT_AST_BOOL("variadic", node->variadic);

	FPRINT_AST_NODE_FINISH;
}
