// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * static_declaration.h -- x86 static declaration target code generation
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_TARGET_X86_STATIC_DECLARATION_H
#define JKCC_TARGET_X86_STATIC_DECLARATION_H


#include <stdio.h>

#include <jkcc/ir.h>


int target_x86_static_declaration(
	FILE                    *stream,
	ir_static_declaration_t *static_declaration);


#endif  /* JKCC_TARGET_X86_STATIC_DECLARATION_H */
