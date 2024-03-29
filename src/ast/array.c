// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * array.c -- array ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast/ast.h>
#include <jkcc/ast/array.h>
#include <jkcc/ast/pointer.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/location.h>


ast_t *ast_array_init(
	ast_t      *type_qualifier_list,
	ast_t      *size,
	location_t *location_start,
	location_t *location_end)
{
	AST_INIT(ast_array_t);

	node->type_qualifier_list = type_qualifier_list;
	node->size                = size;

	node->type = NULL;

	AST_NODE_LOCATION;

	AST_RETURN(AST_ARRAY);
}

void ast_array_free(ast_t *ast)
{
	AST_FREE(ast_array_t);

	if (
		node->type &&
		(*node->type == AST_ARRAY ||
			*node->type == AST_POINTER))
		AST_NODE_FREE(node->type);

	AST_NODE_FREE(node->type_qualifier_list);
	AST_NODE_FREE(node->size);

	free(node);
}

ast_t *ast_array_get_size(ast_t *array)
{
	return OFFSETOF_AST_NODE(array, ast_array_t)->size;
}

ast_t *ast_array_get_type(ast_t *array)
{
	return OFFSETOF_AST_NODE(array, ast_array_t)->type;
}

void ast_array_set_type(
	ast_t *array,
	ast_t *type)
{
	ast_array_t *ast_array = OFFSETOF_AST_NODE(array, ast_array_t);

	ast_array->type = type;
}

void fprint_ast_array(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_array_t);

	FPRINT_AST_MEMBER(ast_node_str[AST_TYPE], node->type);
	FPRINT_AST_MEMBER("type-qualifier-list", node->type_qualifier_list);
	FPRINT_AST_MEMBER("size", node->size);

	FPRINT_AST_NODE_FINISH;
}
