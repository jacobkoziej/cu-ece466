// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * static_declaration.h -- static declaration target code generation
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_TARGET_STATIC_DECLARATION_H
#define JKCC_TARGET_STATIC_DECLARATION_H


#include <jkcc/target/target.h>
#include <jkcc/target/x86/static_declaration.h>

#include <stdio.h>

#include <jkcc/ir.h>


extern int (*const target_static_declaration[TARGET_TOTAL])(
	FILE                    *stream,
	ir_static_declaration_t *static_declaration);


#endif  /* JKCC_TARGET_STATIC_DECLARATION_H */
