// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * sizeof.c -- sizeof basic block
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/bb/sizeof.h>
#include <jkcc/ir/ir.h>
#include <jkcc/private/ir.h>

#include <stdlib.h>
#include <stddef.h>

#include <jkcc/ast.h>
#include <jkcc/constant.h>
#include <jkcc/ht.h>
#include <jkcc/ir.h>
#include <jkcc/vector.h>


int ir_bb_sizeof_gen(
	ir_context_t *ir_context,
	ast_t        *ast)
{
	if (*ast == AST_SIZEOF)     ast = ast_sizeof_get_operand(ast);
	if (*ast == AST_IDENTIFIER) ast = ast_identifier_get_type(ast);

	// TODO: FIX THIS CRIME!
	// we're hard-coding sizes
	size_t size = 4;

	while (*ast != AST_TYPE) {
		switch (*ast) {
			case AST_POINTER:
				size *= 4;
				ast = ast_pointer_get_pointer(ast);
				continue;

			case AST_ARRAY: {
				ast_t *ast_size = ast_array_get_size(ast);

				if (*ast_size != AST_INTEGER_CONSTANT)
					return IR_ERROR_UNKNOWN_AST_NODE;

				const integer_constant_t *integer_constant
					= ast_integer_constant_get_integer_constant(ast_size);

				switch (integer_constant->type) {
					case INT:
						size *= integer_constant->INT;
						break;

					case UNSIGNED_INT:
						size *= integer_constant->UNSIGNED_INT;
						break;

					case LONG_INT:
						size *= integer_constant->LONG_INT;
						break;

					case UNSIGNED_LONG_INT:
						size *= integer_constant->UNSIGNED_LONG_INT;
						break;

					case LONG_LONG_INT:
						size *= integer_constant->LONG_LONG_INT;
						break;

					case UNSIGNED_LONG_LONG_INT:
						size *= integer_constant->UNSIGNED_LONG_LONG_INT;
						break;
				}

				ast = ast_array_get_type(ast);

				break;
			}

			default:
				return IR_ERROR_UNKNOWN_AST_NODE;
		}
	}

	int        ret;
	ir_quad_t *quad;

	ret = ir_quad_mov_gen(
		&quad,
		ir_context->current.dst,
		IR_REG_TYPE_I32,
		size);
	if (ret) return ret;

	if (vector_append(
		&ir_context->ir_bb->quad,
		&quad)) return IR_ERROR_NOMEM;

	uintptr_t  key = ir_context->current.dst;
	void      *val = (void*) IR_REG_TYPE_I32;

	if (ht_insert(
		&ir_context->ir_function->reg.type,
		&key,
		sizeof(key),
		val)) return IR_ERROR_NOMEM;

	ir_context->result = ir_context->current.dst++;
	ir_context->type   = IR_REG_TYPE_I32;

	return 0;
}
