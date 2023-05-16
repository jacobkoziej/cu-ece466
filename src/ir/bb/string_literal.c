// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * string_literal.c -- string_literal basic block
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/bb/string_literal.h>
#include <jkcc/ir/ir.h>
#include <jkcc/private/ir.h>

#include <stdint.h>

#include <jkcc/ast.h>
#include <jkcc/ht.h>
#include <jkcc/ir.h>
#include <jkcc/vector.h>


int ir_bb_string_literal_gen(
	ir_context_t *ir_context,
	ast_t        *ast)
{
	int ret;

	ret = ir_declaration(ir_context, ast);
	if (ret) return ret;

	IR_BB_INIT;

	uintptr_t  key = (uintptr_t) ast;
	void      *val;

	ir_location_t src;

	if (ht_get(&ir_context->static_declaration, &key, sizeof(key), &val))
		return IR_ERROR_UNKNOWN_AST_NODE;

	src.type = IR_LOCATION_STATIC_DECLARATION;
	src.static_declaration = val;

	ir_reg_type_t type = IR_REG_TYPE_PTR;

	ir_quad_t *quad;
	ret = ir_quad_load_gen(&quad, ir_context->current.dst, type, &src);
	if (ret) return ret;

	if (vector_append(&ir_context->ir_bb->quad, &quad))
		return IR_ERROR_NOMEM;

	key = ir_context->current.dst;
	val = (void*) type;

	if (ht_insert(
		&ir_context->ir_function->reg.type,
		&key,
		sizeof(key),
		val)) return IR_ERROR_NOMEM;

	ir_context->result = ir_context->current.dst++;
	ir_context->type   = type;

	return 0;
}
