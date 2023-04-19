// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * function.c -- function ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/function.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/location.h>


ast_t *ast_function_init(
	ast_t        *return_type,
	ast_t        *parameter_list,
	ast_t        *identifier_list,
	bool          variadic,
	location_t   *location_start,
	location_t   *location_end)
{
	AST_INIT(ast_function_t);

	node->return_type     = return_type;
	node->parameter_list  = parameter_list;
	node->identifier_list = identifier_list;
	node->variadic        = variadic;

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

	AST_NODE_FREE(node->parameter_list);
	AST_NODE_FREE(node->identifier_list);

	free(node);
}

void ast_function_prepend_pointer(
	ast_t *function,
	ast_t *pointer)
{
	ast_function_t *ast_function = OFFSETOF_AST_NODE(
		function,
		ast_function_t);

	ast_pointer_append(pointer, ast_function->return_type);

	ast_function->return_type = pointer;
}

void fprint_ast_function(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_function_t);

	FPRINT_AST_MEMBER("return-type", node->return_type);
	FPRINT_AST_MEMBER("parameter-list", node->parameter_list);
	FPRINT_AST_MEMBER("identifier-list", node->identifier_list);
	FPRINT_AST_BOOL("variadic", node->variadic);

	FPRINT_AST_NODE_FINISH;
}
