// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * alloca.c -- stack allocation quad
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/quad/alloca.h>
#include <jkcc/ir/ir.h>
#include <jkcc/private/ir.h>

#include <stdio.h>
#include <stdlib.h>

#include <jkcc/ht.h>
#include <jkcc/ir.h>


void ir_quad_alloca_fprint(FILE *stream, ir_quad_t *ir_quad)
{
	IR_QUAD_FPRINT_BEGIN(ir_quad_alloca_t);

	ir_reg_fprint(stream, quad->dst);
	fprintf(stream, " = alloca ");
	ir_reg_type_fprint(stream, quad->type);
	fprintf(stream, ", ");
	ir_align_fprint(stream, quad->align);

	IR_QUAD_FPRINT_FINISH;
}

int ir_quad_alloca_gen(
	ir_context_t  *ir_context,
	ir_quad_t    **ir_quad,
	ast_t         *type)
{
	IR_QUAD_INIT(ir_quad_alloca_t);

	switch (*type) {
		case AST_POINTER:
			quad->type = IR_REG_TYPE_PTR;
			break;

		default:
			// TODO: FIX THIS CRIME!
			// everything's an i32... sigh
			quad->type = IR_REG_TYPE_I32;
			break;
	}

	quad->dst = ++ir_context->current.dst;

	uintptr_t key;
	uintptr_t val;

	key = quad->dst;
	val = quad->type;
	if (ht_insert(&ir_context->reg.type, &key, sizeof(key), (void*) val))
		goto error_ht_insert_reg_type;

	key = (uintptr_t) type;
	val = quad->type;
	if (ht_insert(&ir_context->reg.lookup, &key, sizeof(key), (void*) val))
		goto error_ht_insert_reg_lookup;

	// TODO: determine alignment
	quad->align = 0;

	IR_QUAD_RETURN(IR_QUAD_ALLOCA);

error_ht_insert_reg_lookup:
	key = quad->dst;
	ht_rm(&ir_context->reg.type, &key, sizeof(key), NULL);

error_ht_insert_reg_type:
	free(quad);

	return IR_ERROR_NOMEM;
}
