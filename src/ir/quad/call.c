// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * call.c -- call quad
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/quad/call.h>
#include <jkcc/ir/ir.h>
#include <jkcc/private/ir.h>

#include <stdio.h>

#include <jkcc/ir.h>


void ir_quad_call_fprint(FILE *stream, ir_quad_t *ir_quad)
{
	IR_QUAD_FPRINT_BEGIN(ir_quad_call_t);

	ir_reg_fprint(stream, quad->dst);
	fprintf(stream, " = call ");
	ir_reg_type_fprint(stream, quad->type);
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

	IR_QUAD_FPRINT_FINISH;
}

int ir_quad_call_gen(
	ir_context_t   *ir_context,
	ir_quad_t     **ir_quad,
	ir_location_t  *src,
	ir_reg_type_t   type)
{
	IR_QUAD_INIT(ir_quad_call_t);

	quad->dst  = ++ir_context->current.dst;
	quad->type =   type;
	quad->src  =  *src;

	IR_QUAD_RETURN(IR_QUAD_CALL);
}
