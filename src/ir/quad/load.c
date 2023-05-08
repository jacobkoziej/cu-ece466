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
	}

	fprintf(stream, ", ");
	ir_align_fprint(stream, quad->align);

	IR_QUAD_FPRINT_FINISH;
}
