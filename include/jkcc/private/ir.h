// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * ir.h -- shitty intermediate representation
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_PRIVATE_IR_H
#define JKCC_PRIVATE_IR_H


#include <jkcc/ir/ir.h>


#define IR_QUAD_INIT(type)                  \
	type *quad = malloc(sizeof(*quad)); \
	if (!quad) return IR_ERROR_NOMEM;

#define IR_QUAD_RETURN(val)        \
	quad->ir_quad = val;       \
	*ir_quad = &quad->ir_quad; \
	return 0;


#endif  /* JKCC_PRIVATE_IR_H */
