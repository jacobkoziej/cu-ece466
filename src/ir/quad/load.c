// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * load.c -- load quad
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/quad/load.h>
#include <jkcc/ir/ir.h>
#include <jkcc/private/ir.h>

#include <stdio.h>

#include <jkcc/ir.h>


void ir_quad_load_fprint(FILE *stream, ir_quad_t *ir_quad)
{
	IR_QUAD_FPRINT_BEGIN(ir_quad_load_t);

	ir_reg_fprint(stream, quad->dst);
	fprintf(stream, " = load ");
	ir_reg_type_fprint(stream, quad->type);
	fprintf(stream, ", ");
	ir_reg_type_fprint(stream, IR_REG_TYPE_PTR);
	fprintf(stream, " ");

	switch (quad->src.type) {
		case IR_LOCATION_REG:
			ir_reg_fprint(stream, quad->src.reg);
			break;

		case IR_LOCATION_EXTERN_DECLARATION:
			ir_extern_declaration_symbol_fprint(
				stream,
				quad->src.extern_declaration);
			break;

		case IR_LOCATION_STATIC_DECLARATION:
			ir_static_declaration_symbol_fprint(
				stream,
				quad->src.static_declaration);
			break;

		case IR_LOCATION_IDENTIFIER:
			fprintf(stream, "@%s", quad->src.identifier->head);
			break;
	}

	fprintf(stream, ", ");
	ir_align_fprint(stream, quad->align);

	IR_QUAD_FPRINT_FINISH;
}

int ir_quad_load_gen(
	ir_quad_t     **ir_quad,
	uintptr_t       dst,
	ir_reg_type_t   type,
	ir_location_t  *src)
{
	IR_QUAD_INIT(ir_quad_load_t);

	quad->dst  =  dst;
	quad->type =  type;
	quad->src  = *src;

	// TODO: determine alignment
	quad->align = 0;

	IR_QUAD_RETURN(IR_QUAD_LOAD);
}
