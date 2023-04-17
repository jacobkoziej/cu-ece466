// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * scope.h -- scope stack
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_SCOPE_H
#define JKCC_SCOPE_H


#include <jkcc/symbol.h>
#include <jkcc/vector.h>


typedef struct scope_s {
	struct {
		symbol_table_t *identifier;
	} current;
	struct {
		vector_t identifier;         // ast_t*
	} history;
} scope_t;


scope_t *scope_init(
	void);
void scope_free(
	scope_t *scope);


#endif  /* JCC_SCOPE_H */
