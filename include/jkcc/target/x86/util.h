// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * util.h -- x86 target code generation utils
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_TARGET_X86_UTIL_H
#define JKCC_TARGET_X86_UTIL_H


#include <stddef.h>
#include <stdint.h>

#include <jkcc/ast.h>


int target_x86_util_ebp_offset(uintptr_t reg, uintptr_t args);
size_t target_x86_util_sizeof(ast_t *type);


#endif  /* JKCC_TARGET_X86_UTIL_H */
