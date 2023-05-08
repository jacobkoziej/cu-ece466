// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * quad.h -- quad ir
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_IR_QUAD_H
#define JKCC_IR_QUAD_H


#include <jkcc/ir/quad/alloca.h>
#include <jkcc/ir/quad/binop.h>

#include <stdint.h>


#define OFFSETOF_IR_QUAD(quad, type) ((type*) (((uintptr_t) quad) - offsetof(type, ir_quad)))


#endif  /* JKCC_IR_QUAD_H */
