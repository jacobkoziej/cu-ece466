// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * store.c -- store basic block
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/bb/store.h>
#include <jkcc/ir/ir.h>
#include <jkcc/private/ir.h>

#include <stdbool.h>
#include <stdint.h>

#include <jkcc/ast.h>
#include <jkcc/ht.h>
#include <jkcc/ir.h>


int ir_bb_store_gen(
	ir_context_t *ir_context,
	ast_t        *ast)
{
	struct {
		uintptr_t     src;
		ir_reg_type_t type;
		uintptr_t     dst;
	} store = {0};

	struct {
		ir_quad_binop_op_t op;
		ir_reg_type_t      type;
		struct {
			uintptr_t     reg;
			ir_reg_type_t type;
		} lhs, rhs;
	} binop = {0};

	binop.type = IR_REG_TYPE_I32;

	uint_fast16_t  assignment = ast_assignment_get_assignment(ast);
	ast_t         *lvalue     = ast_assignment_get_lvalue(ast);
	ast_t         *rvalue     = ast_assignment_get_rvalue(ast);

	int        ret;
	ir_quad_t *quad;
	uintptr_t  key;
	void      *val;

	IR_BB_INIT;

	switch (assignment) {
		case AST_ASSIGNMENT_ASSIGNMENT:
			break;

		case AST_ASSIGNMENT_COMPOUND_MULTIPLICATION:
			binop.op = IR_QUAD_BINOP_MUL;
			goto compound_assignment;

		case AST_ASSIGNMENT_COMPOUND_DIVISION:
			binop.op = IR_QUAD_BINOP_DIV;
			goto compound_assignment;

		case AST_ASSIGNMENT_COMPOUND_MODULO:
			binop.op = IR_QUAD_BINOP_MOD;
			goto compound_assignment;

		case AST_ASSIGNMENT_COMPOUND_ADDITION:
			binop.op = IR_QUAD_BINOP_ADD;
			goto compound_assignment;

		case AST_ASSIGNMENT_COMPOUND_SUBTRACTION:
			binop.op = IR_QUAD_BINOP_SUB;
			goto compound_assignment;

		case AST_ASSIGNMENT_COMPOUND_LBITSHIFT:
			binop.op = IR_QUAD_BINOP_LSL;
			goto compound_assignment;

		case AST_ASSIGNMENT_COMPOUND_RBITSHIFT:
			binop.op = IR_QUAD_BINOP_LSR;
			goto compound_assignment;

		case AST_ASSIGNMENT_COMPOUND_AND:
			binop.op = IR_QUAD_BINOP_AND;
			goto compound_assignment;

		case AST_ASSIGNMENT_COMPOUND_XOR:
			binop.op = IR_QUAD_BINOP_EOR;
			goto compound_assignment;

		case AST_ASSIGNMENT_COMPOUND_OR:
			binop.op = IR_QUAD_BINOP_OOR;
			goto compound_assignment;

		default:
			return IR_ERROR_UNKNOWN_AST_NODE;
	}

	ret = IR_BB_GEN(ir_context, rvalue);
	if (ret) return ret;
	store.src  = ir_context->result;
	store.type = ir_context->type;

assignment:
	ir_context->lvalue = true;
	ret = IR_BB_GEN(ir_context, lvalue);
	if (ret) return ret;
	store.dst = ir_context->result;

	ret = ir_quad_store_gen(
		&quad,
		store.src,
		store.type,
		store.dst);
	if (ret) return ret;

	if (vector_append(&ir_context->ir_bb->quad, &quad))
		goto error_vector_append_ir_bb_quad;

	ir_context->result = store.src;

	return 0;

compound_assignment:
	ret = IR_BB_GEN(ir_context, lvalue);
	if (ret) return ret;
	binop.lhs.reg  = ir_context->result;
	binop.lhs.type = ir_context->type;

	ret = IR_BB_GEN(ir_context, rvalue);
	if (ret) return ret;
	binop.rhs.reg  = ir_context->result;
	binop.rhs.type = ir_context->type;

	// integer "promotion"
	if (binop.lhs.type != binop.rhs.type) {
		ret = ir_quad_mov_gen(
				&quad,
				ir_context->current.dst,
				IR_REG_TYPE_I32,
				4);
		if (ret) return ret;

		if (vector_append(
			&ir_context->ir_bb->quad,
			&quad)) return IR_ERROR_NOMEM;

		ir_context->result = ir_context->current.dst++;
		ir_context->type   = IR_REG_TYPE_I32;

		uintptr_t src = (binop.lhs.type == IR_REG_TYPE_I32)
			? binop.lhs.reg
			: binop.rhs.reg;

		ret = ir_quad_binop_gen(
			&quad,
			ir_context->current.dst,
			IR_QUAD_BINOP_MUL,
			IR_REG_TYPE_I32,
			src,
			ir_context->result);
		if (ret) return ret;

		if (vector_append(
			&ir_context->ir_bb->quad,
			&quad)) return IR_ERROR_NOMEM;

		ir_context->result = ir_context->current.dst++;
		ir_context->type   = IR_REG_TYPE_PTR;

		if (binop.lhs.type == IR_REG_TYPE_I32) {
			binop.lhs.reg  = ir_context->result;
			binop.lhs.type = ir_context->type;
		} else {
			binop.rhs.reg  = ir_context->result;
			binop.rhs.type = ir_context->type;
		}

		binop.type = IR_REG_TYPE_PTR;
	}

	ret = ir_quad_binop_gen(
		&quad,
		ir_context->current.dst,
		binop.op,
		binop.type,
		binop.lhs.reg,
		binop.rhs.reg);
	if (ret) return ret;

	if (vector_append(&ir_context->ir_bb->quad, &quad))
		goto error_vector_append_ir_bb_quad;

	key = ir_context->current.dst;
	val = (void*) binop.type;

	if (ht_insert(
		&ir_context->ir_function->reg.type,
		&key,
		sizeof(key),
		val)) goto error_ht_insert_reg_type;

	store.src  = ir_context->result = ir_context->current.dst++;
	store.type = ir_context->type;

	goto assignment;

error_ht_insert_reg_type:
	vector_pop(&ir_context->ir_bb->quad, NULL);

error_vector_append_ir_bb_quad:
	free(quad);

	return IR_ERROR_NOMEM;
}
