// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * bb.c -- basic block ir
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/bb.h>
#include <jkcc/ir/ir.h>

#include <stddef.h>
#include <stdlib.h>

#include <jkcc/vector.h>


ir_bb_t *ir_bb_alloc(size_t id)
{
	ir_bb_t *ir_bb = malloc(sizeof(*ir_bb));
	if (!ir_bb) return NULL;

	if (vector_init(&ir_bb->quad, sizeof(ir_quad_t*), 0)) goto error;

	ir_bb->id = id;

	return ir_bb;

error:
	free(ir_bb);

	return NULL;
}

void ir_bb_free(ir_bb_t *ir_bb)
{
	if (!ir_bb) return;

	ir_quad_t **ir_quad = ir_bb->quad.buf;
	for (size_t i = 0; i < ir_bb->quad.use; i++)
		(void) ir_quad;

	vector_free(&ir_bb->quad);

	free(ir_bb);
}
