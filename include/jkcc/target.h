// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * target.h -- garbage target code generation
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_TARGET_H
#define JKCC_TARGET_H


#include <jkcc/target/extern_declaration.h>
#include <jkcc/target/static_declaration.h>
#include <jkcc/target/target.h>
#include <jkcc/target/util.h>
#include <jkcc/target/x86.h>

#include <stdio.h>

#include <jkcc/ir.h>


int target_gen(FILE *stream, target_t target, ir_unit_t *ir_unit);


#endif  /* JKCC_TARGET_H */
