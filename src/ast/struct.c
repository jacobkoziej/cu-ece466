// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * struct.c -- struct ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast/ast.h>
#include <jkcc/ast/struct.h>
#include <jkcc/private/ast.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/location.h>


ast_t *ast_struct_init(
	ast_t          *tag,
	ast_t          *declaration_list,
	symbol_table_t *members,
	bool            declaration,
	uint_fast8_t    type,
	location_t     *location_start,
	location_t     *location_end)
{
	AST_INIT(ast_struct_t);

	node->tag              = tag;
	node->declaration_list = declaration_list;
	node->members          = members;
	node->declaration      = declaration;
	node->type             = type;

	node->definition = NULL;

	AST_NODE_LOCATION;

	AST_RETURN(AST_STRUCT);
}

void ast_struct_free(ast_t *ast)
{
	AST_FREE(ast_struct_t);

	AST_NODE_FREE(node->tag);
	AST_NODE_FREE(node->declaration_list);

	free(node);
}

bool ast_struct_get_declaration(
	ast_t *ast)
{
	ast_struct_t *ast_struct = OFFSETOF_AST_NODE(ast, ast_struct_t);

	return ast_struct->declaration;
}

ast_t *ast_struct_get_definition(
	ast_t *ast)
{
	ast_struct_t *ast_struct = OFFSETOF_AST_NODE(ast, ast_struct_t);

	return ast_struct->definition;
}

void ast_struct_set_definition(
	ast_t *ast,
	bool   declaration,
	ast_t *definition)
{
	do {
		ast_struct_t *ast_struct = OFFSETOF_AST_NODE(ast, ast_struct_t);

		ast = ast_struct->definition;

		// replace references to incomplete type
		// with struct definition
		ast_struct->definition  = definition;
		ast_struct->declaration = declaration;
	} while (ast);
}

void ast_struct_set_declaration_list(
	ast_t *ast,
	ast_t *declaration_list)
{
	ast_struct_t *ast_struct = OFFSETOF_AST_NODE(ast, ast_struct_t);

	ast_struct->declaration_list= declaration_list;
}

void ast_struct_set_symbol_table(
	ast_t          *ast,
	symbol_table_t *members)
{
	ast_struct_t *ast_struct = OFFSETOF_AST_NODE(ast, ast_struct_t);

	ast_struct->members = members;
}

void fprint_ast_struct(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_struct_t);

	FPRINT_AST_MEMBER("tag", node->tag);
	FPRINT_AST_MEMBER("declaration-list", node->declaration_list);

	const char *type;
	switch (node->type) {
		case AST_STRUCT_STRUCT:
			type = "struct";
			break;

		case AST_STRUCT_UNION:
			type = "union";
			break;

		default:
			type = "(unknown)";
			break;
	}

	FPRINT_AST_FIELD("type", type);

	FPRINT_AST_NODE_FINISH;
}
