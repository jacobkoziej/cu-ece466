// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * call.c -- call basic block
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/bb/call.h>
#include <jkcc/ir/ir.h>
#include <jkcc/private/ir.h>

#include <stdint.h>

#include <jkcc/ast.h>
#include <jkcc/ht.h>
#include <jkcc/ir.h>
#include <jkcc/vector.h>


int ir_bb_call_gen(
	ir_context_t *ir_context,
	ast_t        *ast)
{
	ast_t *ast_expression    = ast_call_get_expression(ast);
	ast_t *ast_argument_list = ast_call_get_argument_list(ast);

	// we sadly don't support function poitners
	if (*ast_expression != AST_IDENTIFIER)
		return IR_ERROR_UNKNOWN_AST_NODE;

	int        ret;
	ir_quad_t *quad;

	uintptr_t  key;
	void      *val;

	ir_reg_type_t type;

	IR_BB_INIT;

	if (ast_argument_list) {
		vector_t *list = ast_list_get_list(ast_argument_list);

		ast_t **argument = list->buf;
		for (size_t i = list->use - 1; i != SIZE_MAX; i--) {
			ret = IR_BB_GEN(ir_context, argument[i]);
			if (ret) return ret;

			key = ir_context->result;
			ht_get(
				&ir_context->ir_function->reg.type,
				&key,
				sizeof(key),
				&val);

			type = (ir_reg_type_t) val;

			ret = ir_quad_arg_gen(
				&quad,
				i,
				ir_context->result,
				type);
			if (ret) return ret;

			if (vector_append(
				&ir_context->ir_bb->quad,
				&quad))
				return IR_ERROR_NOMEM;
		}
	}

	// TODO: determine return types
	type = IR_REG_TYPE_I32;

	ir_location_t src = {
		.type = IR_LOCATION_IDENTIFIER,
		.identifier = ast_identifier_get_string(ast_expression),
	};

	ret = ir_quad_call_gen(
		&quad,
		ir_context->current.dst,
		type,
		&src);
	if (ret) return ret;

	if (vector_append(
		&ir_context->ir_bb->quad,
		&quad))
		return IR_ERROR_NOMEM;

	key = ir_context->current.dst;
	val = (void*) type;

	if (ht_insert(
		&ir_context->ir_function->reg.type,
		&key,
		sizeof(key),
		val)) return IR_ERROR_NOMEM;

	ir_context->result = ir_context->current.dst++;

	// a call terminates a basic block
	ret = ir_quad_br_gen(&quad, IR_QUAD_BR_AL, ir_context->current.bb);
	if (ret) return ret;

	if (vector_append(
		&ir_context->ir_bb->quad,
		&quad))
		return IR_ERROR_NOMEM;

	// trigger new basic block to be generated
	ir_context->ir_bb = NULL;
	IR_BB_INIT;

	return 0;
}
