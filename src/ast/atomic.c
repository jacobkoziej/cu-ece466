// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * atomic.c -- atomic ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast/ast.h>
#include <jkcc/ast/atomic.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/location.h>


ast_t *ast_atomic_init(
	ast_t       *operand,
	location_t  *location_start,
	location_t  *location_end,
	const char **error)
{
	ast_type_t *type = OFFSETOF_AST_NODE(operand, ast_type_t);

	// TODO: add checks for array, function, and atomic types

	// atomic type shall not refer to a qualifier type
	if (type->type_qualifier) {
		*error = "atomic type shall not refer to a qualifier type";
		return NULL;
	}

	AST_INIT(ast_atomic_t);

	node->operand = operand;

	AST_NODE_LOCATION;

	AST_RETURN(AST_ATOMIC);
}

void ast_atomic_free(ast_t *ast)
{
	AST_FREE(ast_atomic_t);

	AST_NODE_FREE(node->operand);

	free(node);
}

void fprint_ast_atomic(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_atomic_t);

	FPRINT_AST_MEMBER("operand", node->operand);

	FPRINT_AST_NODE_FINISH;
}
