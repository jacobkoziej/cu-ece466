// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * store.c -- store quad
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/quad/store.h>
#include <jkcc/ir/ir.h>
#include <jkcc/private/ir.h>

#include <stdint.h>

#include <jkcc/ir.h>


void ir_quad_store_fprint(FILE *stream, ir_quad_t *ir_quad)
{
	IR_QUAD_FPRINT_BEGIN(ir_quad_store_t);

	fprintf(stream, "store ");
	ir_reg_type_fprint(stream, quad->type);
	fprintf(stream, " ");
	ir_reg_fprint(stream, quad->src);
	fprintf(stream, ", ");
	ir_reg_type_fprint(stream, IR_REG_TYPE_PTR);
	fprintf(stream, " ");

	switch (quad->dst.type) {
		case IR_LOCATION_REG:
			ir_reg_fprint(stream, quad->dst.reg);
			break;

		case IR_LOCATION_EXTERN_DECLARATION:
			ir_extern_declaration_symbol_fprint(
				stream,
				quad->dst.extern_declaration);
			break;

		case IR_LOCATION_STATIC_DECLARATION:
			ir_static_declaration_symbol_fprint(
				stream,
				quad->dst.static_declaration);
			break;
	}

	fprintf(stream, ", ");
	ir_align_fprint(stream, quad->align);

	IR_QUAD_FPRINT_FINISH;
}

int ir_quad_store_gen(
	ir_quad_t     **ir_quad,
	ir_reg_type_t   type,
	uintptr_t       src,
	ir_location_t  *dst)
{
	IR_QUAD_INIT(ir_quad_store_t);

	quad->dst  = *dst;
	quad->type =  type;
	quad->src  =  src;

	// TODO: determine alignment
	quad->align = 0;

	IR_QUAD_RETURN(IR_QUAD_STORE);
}
