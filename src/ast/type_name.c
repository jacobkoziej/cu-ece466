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

#include <jkcc/lexer.h>


ast_t *ast_type_name_init(
	ast_t      *specifier_qualifier_list,
	ast_t      *abstract_declarator,
	location_t *location_start,
	location_t *location_end)
{
	AST_INIT(ast_type_name_t);

	node->specifier_qualifier_list =  specifier_qualifier_list;
	node->abstract_declarator      =  abstract_declarator;

	node->location.file  = location_start->file;
	node->location.start = location_start->start;

	node->location.end = (location_end)
		? location_end->end
		: location_start->end;

	AST_RETURN(AST_TYPE_NAME);
}

void ast_type_name_free(ast_t *ast)
{
	AST_FREE(ast_type_name_t);

	AST_NODE_FREE(node->specifier_qualifier_list);
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

	FPRINT_AST_MEMBER(
		ast_node_str[AST_SPECIFIER_QUALIFIER_LIST],
		node->specifier_qualifier_list);
	/* TODO: AST_ABSTRACT_DECLARATOR
	FPRINT_AST_MEMBER(
		ast_node_str[AST_ABSTRACT_DECLARATOR],
		node->abstract_declarator);
	*/

	FPRINT_AST_NODE_FINISH;
}
