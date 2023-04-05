// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * type_name.c -- type_name ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/type_name.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/location.h>


ast_t *ast_type_name_init(
	ast_t      *type,
	ast_t      *abstract_declarator,
	location_t *location_start,
	location_t *location_end)
{
	AST_INIT(ast_type_name_t);

	node->type                = type;
	node->abstract_declarator = abstract_declarator;

	AST_NODE_LOCATION;

	AST_RETURN(AST_TYPE_NAME);
}

void ast_type_name_free(ast_t *ast)
{
	AST_FREE(ast_type_name_t);

	AST_NODE_FREE(node->type);
	AST_NODE_FREE(node->abstract_declarator);

	free(node);
}

void fprint_ast_type_name(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_type_name_t);

	FPRINT_AST_MEMBER(ast_node_str[AST_TYPE], node->type);
	/* TODO: AST_ABSTRACT_DECLARATOR
	FPRINT_AST_MEMBER(
		ast_node_str[AST_ABSTRACT_DECLARATOR],
		node->abstract_declarator);
	*/

	FPRINT_AST_NODE_FINISH;
}
