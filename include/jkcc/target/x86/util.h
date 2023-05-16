// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * util.h -- x86 target code generation utils
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_TARGET_X86_UTIL_H
#define JKCC_TARGET_X86_UTIL_H


#include <stddef.h>

#include <jkcc/ast.h>


size_t target_x86_util_sizeof(ast_t *type);


#endif  /* JKCC_TARGET_X86_UTIL_H */
