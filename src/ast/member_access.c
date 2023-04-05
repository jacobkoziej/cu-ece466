// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * member_access.c -- member_access ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/member_access.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/location.h>


ast_t *ast_member_access_init(
	ast_t      *operand,
	ast_t      *identifier,
	location_t *location_start,
	location_t *location_end)
{
	AST_INIT(ast_member_access_t);

	node->operand    = operand;
	node->identifier = identifier;

	AST_NODE_LOCATION;

	AST_RETURN(AST_MEMBER_ACCESS);
}

void ast_member_access_free(ast_t *ast)
{
	AST_FREE(ast_member_access_t);

	AST_NODE_FREE(node->operand);
	AST_NODE_FREE(node->identifier);

	free(node);
}

void fprint_ast_member_access(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_member_access_t);

	FPRINT_AST_MEMBER("operand", node->operand);
	FPRINT_AST_MEMBER(ast_node_str[AST_IDENTIFIER], node->identifier);

	FPRINT_AST_NODE_FINISH;
}
