// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * ast.c -- abstract syntax tree
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <jkcc/parser.h>
#include <jkcc/string.h>


ast_t *ast_identifier_init(string_t *identifier, location_t *location)
{
	ast_identifier_t *node = malloc(sizeof(*node));
	if (!node) return NULL;

	memcpy(&node->identifier, identifier, sizeof(*identifier));
	memcpy(&node->location, location, sizeof(*location));

	node->ast = AST_IDENTIFIER;

	return &node->ast;
}
