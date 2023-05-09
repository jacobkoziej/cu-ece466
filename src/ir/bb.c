// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * bb.c -- basic block ir
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/bb.h>
#include <jkcc/ir/ir.h>

#include <stddef.h>
#include <stdlib.h>

#include <jkcc/ast.h>
#include <jkcc/vector.h>


int (*const ir_bb_gen[AST_NODES_TOTAL])(
	ir_context_t *ir_context,
	ast_t        *ast) = {
	[AST_ADDRESSOF]                = ir_bb_unknown_gen,
	[AST_ALIGNAS]                  = ir_bb_unknown_gen,
	[AST_ALIGNOF]                  = ir_bb_unknown_gen,
	[AST_ARRAY]                    = ir_bb_unknown_gen,
	[AST_ASSIGNMENT]               = ir_bb_unknown_gen,
	[AST_ATOMIC]                   = ir_bb_unknown_gen,
	[AST_BINARY_OPERATOR]          = ir_bb_unknown_gen,
	[AST_BREAK]                    = ir_bb_unknown_gen,
	[AST_CALL]                     = ir_bb_unknown_gen,
	[AST_CASE]                     = ir_bb_unknown_gen,
	[AST_CAST]                     = ir_bb_unknown_gen,
	[AST_CHARACTER_CONSTANT]       = ir_bb_unknown_gen,
	[AST_CONTINUE]                 = ir_bb_unknown_gen,
	[AST_DECLARATION]              = ir_bb_declaration_gen,
	[AST_DEREFERENCE]              = ir_bb_unknown_gen,
	[AST_EMPTY]                    = ir_bb_unknown_gen,
	[AST_EXPRESSION]               = ir_bb_unknown_gen,
	[AST_FLOATING_CONSTANT]        = ir_bb_unknown_gen,
	[AST_FOR]                      = ir_bb_unknown_gen,
	[AST_FUNCTION]                 = ir_bb_unknown_gen,
	[AST_FUNCTION_SPECIFIER]       = ir_bb_unknown_gen,
	[AST_GENERIC_ASSOCIATION]      = ir_bb_unknown_gen,
	[AST_GENERIC_ASSOCIATION_LIST] = ir_bb_unknown_gen,
	[AST_GENERIC_SELECTION]        = ir_bb_unknown_gen,
	[AST_GOTO]                     = ir_bb_unknown_gen,
	[AST_IDENTIFIER]               = ir_bb_unknown_gen,
	[AST_IF]                       = ir_bb_unknown_gen,
	[AST_INTEGER_CONSTANT]         = ir_bb_unknown_gen,
	[AST_LABEL]                    = ir_bb_unknown_gen,
	[AST_LIST]                     = ir_bb_unknown_gen,
	[AST_MEMBER_ACCESS]            = ir_bb_unknown_gen,
	[AST_POINTER]                  = ir_bb_unknown_gen,
	[AST_RETURN]                   = ir_bb_unknown_gen,
	[AST_SIZEOF]                   = ir_bb_unknown_gen,
	[AST_STATIC_ASSERT]            = ir_bb_unknown_gen,
	[AST_STORAGE_CLASS_SPECIFIER]  = ir_bb_unknown_gen,
	[AST_STRING_LITERAL]           = ir_bb_unknown_gen,
	[AST_STRUCT]                   = ir_bb_unknown_gen,
	[AST_SWITCH]                   = ir_bb_unknown_gen,
	[AST_TERNARY_OPERATOR]         = ir_bb_unknown_gen,
	[AST_TRANSLATION_UNIT]         = ir_bb_unknown_gen,
	[AST_TYPE]                     = ir_bb_unknown_gen,
	[AST_TYPE_QUALIFIER]           = ir_bb_unknown_gen,
	[AST_TYPE_SPECIFIER]           = ir_bb_unknown_gen,
	[AST_UNARY_OPERATOR]           = ir_bb_unknown_gen,
	[AST_WHILE]                    = ir_bb_unknown_gen,
};


ir_bb_t *ir_bb_alloc(size_t id)
{
	ir_bb_t *ir_bb = malloc(sizeof(*ir_bb));
	if (!ir_bb) return NULL;

	if (vector_init(&ir_bb->quad, sizeof(ir_quad_t*), 0)) goto error;

	ir_bb->id = id;

	return ir_bb;

error:
	free(ir_bb);

	return NULL;
}

void ir_bb_free(ir_bb_t *ir_bb)
{
	if (!ir_bb) return;

	ir_quad_t **ir_quad = ir_bb->quad.buf;
	for (size_t i = 0; i < ir_bb->quad.use; i++)
		(void) ir_quad;

	vector_free(&ir_bb->quad);

	free(ir_bb);
}

int ir_bb_unknown_gen(
	ir_context_t *ir_context,
	ast_t        *ast)
{
	(void) ir_context;
	(void) ast;

	return IR_ERROR_UNKNOWN_AST_NODE;
}
