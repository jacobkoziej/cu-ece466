// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * pointer.c -- pointer ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast/ast.h>
#include <jkcc/ast/pointer.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/location.h>


void ast_pointer_append(
	ast_t *pointer,
	ast_t *type)
{
	ast_pointer_t *ast_pointer;

	do {
		ast_pointer = OFFSETOF_AST_NODE(pointer, ast_pointer_t);
		pointer = ast_pointer->pointer;
	} while (ast_pointer->pointer);

	ast_pointer->pointer = type;
}

ast_t *ast_pointer_init(
	ast_t      *pointer,
	ast_t      *type_qualifier_list,
	location_t *location)
{
	AST_INIT(ast_pointer_t);

	node->pointer             =  pointer;
	node->type_qualifier_list =  type_qualifier_list;
	node->location            = *location;

	AST_RETURN(AST_POINTER);
}

void ast_pointer_free(ast_t *ast)
{
	AST_FREE(ast_pointer_t);

	// ast_pointer_t only references ast_type_t,
	// we want to avoid a double free here
	if (node->pointer)
		switch (*node->pointer) {
			case AST_ARRAY:
			case AST_FUNCTION:
			case AST_POINTER:
				AST_NODE_FREE(node->pointer);
				break;

			default:
				break;
		}

	AST_NODE_FREE(node->type_qualifier_list);

	free(node);
}

void fprint_ast_pointer(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_pointer_t);

	FPRINT_AST_MEMBER(ast_node_str[AST_POINTER], node->pointer);
	FPRINT_AST_MEMBER("type-qualifier-list", node->type_qualifier_list);

	FPRINT_AST_NODE_FINISH;
}
