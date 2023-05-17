// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * symbol.c -- symbol basic block
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/bb/symbol.h>
#include <jkcc/ir/ir.h>
#include <jkcc/private/ir.h>

#include <stdint.h>

#include <jkcc/ast.h>
#include <jkcc/ht.h>
#include <jkcc/ir.h>


int ir_bb_symbol_gen(
	ir_context_t *ir_context,
	ast_t        *ast)
{
	uintptr_t  key = (uintptr_t) ast_identifier_get_type(ast);
	void      *val;

	ir_location_t  src;
	ir_reg_type_t  type;
	ir_quad_t     *quad;
	int            ret;

	if (ht_get(
		&ir_context->ir_function->reg.lookup,
		&key,
		sizeof(key),
		&val)
	) {
		// TODO: FIX THIS CRIME!
		// to simplify target code generation
		// static symbols addresses are loaded into
		// a register and then used

		if (!ht_get(
			&ir_context->static_declaration,
			&key,
			sizeof(key),
			&val)
		) {
			src.type = IR_LOCATION_STATIC_DECLARATION;
			src.static_declaration = val;

			goto src_set;
		}

		if (!ht_get(
				&ir_context->extern_declaration,
				&key,
				sizeof(key),
				&val)
		) {
			src.type = IR_LOCATION_EXTERN_DECLARATION;
			src.extern_declaration = val;

			goto src_set;
		}

		return IR_ERROR_UNKNOWN_AST_NODE;

src_set:
		// HAK <3
		if (ir_context->lvalue) {
			type = IR_REG_TYPE_LEA;
			ir_context->lea = true;
		} else {
			type = IR_REG_TYPE_PTR;
		}

		ret = ir_quad_load_gen(
			&quad,
			ir_context->current.dst,
			type,
			&src);
		if (ret) return ret;

		if (vector_append(&ir_context->ir_bb->quad, &quad))
			return IR_ERROR_NOMEM;

		// TODO: FIX THIS CRIME!
		// we're assuming that we're either an int or a pointer
		type = (*(ast_t*)key == AST_TYPE)
			? IR_REG_TYPE_I32
			: IR_REG_TYPE_PTR;

		key = ir_context->current.dst;
		val = (void*) type;

		if (ht_insert(
			&ir_context->ir_function->reg.type,
			&key,
			sizeof(key),
			val)) return IR_ERROR_NOMEM;

		val = (void*) ir_context->current.dst++;
	}

	if (ir_context->lvalue) {
		ir_context->lvalue = false;
		ir_context->result = (uintptr_t) val;
		ir_context->type   = type;
		return 0;
	}

	src.type = IR_LOCATION_REG;
	src.reg  = (uintptr_t) val;

	key = (uintptr_t) val;
	ht_get(&ir_context->ir_function->reg.type, &key, sizeof(key), &val);

	type = (uintptr_t) val;

	ret = ir_quad_load_gen(&quad, ir_context->current.dst, type, &src);
	if (ret) return ret;

	if (vector_append(&ir_context->ir_bb->quad, &quad))
		goto error_vector_append_ir_bb_quad;

	key = ir_context->current.dst;
	// TODO: determine result types
	val = IR_REG_TYPE_I32;

	if (ht_insert(
		&ir_context->ir_function->reg.type,
		&key,
		sizeof(key),
		val)) goto error_ht_insert_reg_type;

	ir_context->result = ir_context->current.dst++;
	ir_context->type   = type;

	return 0;

error_ht_insert_reg_type:
	vector_pop(&ir_context->ir_bb->quad, NULL);

error_vector_append_ir_bb_quad:
	free(quad);

	return IR_ERROR_NOMEM;
}
