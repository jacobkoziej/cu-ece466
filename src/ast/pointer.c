// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * pointer.c -- pointer ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/pointer.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/lexer.h>


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

	AST_NODE_FREE(node->pointer);
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

	FPRINT_AST_MEMBER(
		ast_node_str[AST_POINTER],
		node->pointer);
	FPRINT_AST_MEMBER(
		ast_node_str[AST_TYPE_QUALIFIER_LIST],
		node->type_qualifier_list);

	FPRINT_AST_NODE_FINISH;
}
