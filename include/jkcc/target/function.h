// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * function.h -- function target code generation
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_TARGET_FUNCTION_H
#define JKCC_TARGET_FUNCTION_H


#include <jkcc/target/target.h>

#include <stdio.h>

#include <jkcc/ir.h>


extern int (*const target_function[TARGET_TOTAL])(
	FILE          *stream,
	ir_function_t *function);


#endif  /* JKCC_TARGET_FUNCTION_H */
