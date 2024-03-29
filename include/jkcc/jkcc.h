// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * jkcc.h -- Jacob Koziej's C Compiler
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_JKCC_H
#define JKCC_JKCC_H


#include <stddef.h>

#include <jkcc/trace.h>
#include <jkcc/vector.h>


typedef struct jkcc_config_s {
	unsigned ansi_sgr_stdout : 1;
	unsigned ansi_sgr_stderr : 1;
	unsigned clean_exit      : 1;
	unsigned print_ast       : 1;
	unsigned print_ir        : 1;
	unsigned trace           : 1;
} jkcc_config_t;

typedef struct jkcc_s {
	const char    **file;
	size_t          file_count;
	vector_t        translation_unit;  // ast_t*
	vector_t        ir_unit;           // ir_unit_t*
	trace_t         trace;
	jkcc_config_t   config;
} jkcc_t;


#endif  /* JKCC_JKCC_H */
