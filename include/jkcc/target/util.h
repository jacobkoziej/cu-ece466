// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * util.h -- target code generation utils
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_TARGET_UTIL_H
#define JKCC_TARGET_UTIL_H


#include <jkcc/target/x86/util.h>

#include <stddef.h>
#include <stdio.h>

#include <jkcc/ast.h>


size_t target_util_string_literal(FILE *stream, ast_t *ast);


#endif  /* JKCC_TARGET_UTIL_H */
