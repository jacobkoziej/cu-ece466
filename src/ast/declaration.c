// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * declaration.c -- declaration ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast/ast.h>
#include <jkcc/ast/declaration.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/location.h>


ast_t *ast_declaration_init(
	ast_t         *type,
	ast_t         *identifier,
	ast_t         *initializer,
	uint_fast8_t   storage_class,
	location_t    *location_start,
	location_t    *location_end)
{
	AST_INIT(ast_declaration_t);

	node->type          = type;
	node->identifier    = identifier;
	node->initializer   = initializer;
	node->storage_class = storage_class;

	AST_NODE_LOCATION;

	AST_RETURN(AST_DECLARATION);
}

void ast_declaration_free(ast_t *ast)
{
	AST_FREE(ast_declaration_t);

	switch (*node->type) {
		case AST_ARRAY:
		case AST_FUNCTION:
		case AST_POINTER:
			AST_NODE_FREE(node->type);
			break;

		default:
			break;
	}
	AST_NODE_FREE(node->identifier);
	AST_NODE_FREE(node->initializer);

	free(node);
}

ast_t *ast_declaration_get_identifier(ast_t *ast)
{
	return OFFSETOF_AST_NODE(ast, ast_declaration_t)->identifier;
}

uint_fast8_t ast_declaration_get_storage_class(ast_t *ast)
{
	return OFFSETOF_AST_NODE(ast, ast_declaration_t)->storage_class;
}

ast_t *ast_declaration_get_type(ast_t *ast)
{
	return OFFSETOF_AST_NODE(ast, ast_declaration_t)->type;
}

void fprint_ast_declaration(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_declaration_t);

	FPRINT_AST_MEMBER(ast_node_str[AST_TYPE], node->type);
	FPRINT_AST_MEMBER(ast_node_str[AST_IDENTIFIER], node->identifier);
	FPRINT_AST_MEMBER("initializer", node->initializer);

	const char *storage_class;
	switch (node->storage_class) {
		case AST_DECLARATION_IMPLICIT_EXTERN:
			storage_class = "implicit-extern";
			break;

		case AST_DECLARATION_EXPLICIT_EXTERN:
			storage_class = "explicit-extern";
			break;

		case AST_DECLARATION_STATIC:
			storage_class = "static";
			break;

		case AST_DECLARATION_AUTO:
			storage_class = "auto";
			break;

		case AST_DECLARATION_REGISTER:
			storage_class = "register";
			break;

		case AST_DECLARATION_ARGUMENT:
			storage_class = "argument";
			break;

		default:
			storage_class = "(unknown)";
			break;
	}

	FPRINT_AST_FIELD("storage-class", storage_class);

	FPRINT_AST_NODE_FINISH;
}
