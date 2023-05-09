// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * ir.h -- shitty intermediate representation
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_PRIVATE_IR_H
#define JKCC_PRIVATE_IR_H


#include <jkcc/ir.h>

#include <stdio.h>
#include <stdlib.h>


#define IR_BB_INIT                                                       \
	if (!ir_context->ir_bb) {                                        \
		ir_context->ir_bb = ir_bb_alloc(ir_context->current.bb); \
		if (!ir_context->ir_bb) return IR_ERROR_NOMEM;           \
                                                                         \
		if (vector_append(                                       \
			&ir_context->ir_function->bb,                    \
			&ir_context->ir_bb)                              \
		) {                                                      \
			ir_bb_free(ir_context->ir_bb);                   \
			ir_context->ir_bb = NULL;                        \
			return IR_ERROR_NOMEM;                           \
		}                                                        \
                                                                         \
		++ir_context->current.bb;                                \
	}

#define IR_QUAD_INIT(type)                  \
	type *quad = malloc(sizeof(*quad)); \
	if (!quad) return IR_ERROR_NOMEM;

#define IR_QUAD_RETURN(val)        \
	quad->ir_quad = val;       \
	*ir_quad = &quad->ir_quad; \
	return 0;

#define IR_QUAD_FPRINT_BEGIN(type)                    \
	type *quad = OFFSETOF_IR_QUAD(ir_quad, type); \
	fprintf(stream, "\t");

#define IR_QUAD_FPRINT_FINISH  \
	fprintf(stream, "\n");



#endif  /* JKCC_PRIVATE_IR_H */
