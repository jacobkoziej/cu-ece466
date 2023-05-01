// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * label.c -- label ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast/ast.h>
#include <jkcc/ast/label.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/location.h>


ast_t *ast_label_init(
	ast_t      *identifier,
	ast_t      *statement,
	location_t *location_start,
	location_t *location_end)
{
	AST_INIT(ast_label_t);

	node->identifier = identifier;
	node->statement  = statement;

	AST_NODE_LOCATION;

	AST_RETURN(AST_LABEL);
}

void ast_label_free(ast_t *ast)
{
	AST_FREE(ast_label_t);

	AST_NODE_FREE(node->identifier);
	AST_NODE_FREE(node->statement);

	free(node);
}

void fprint_ast_label(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_label_t);

	FPRINT_AST_MEMBER(ast_node_str[AST_IDENTIFIER], node->identifier);
	FPRINT_AST_MEMBER("statement", node->statement);

	FPRINT_AST_NODE_FINISH;
}
