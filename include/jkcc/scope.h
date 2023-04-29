// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * scope.h -- scope stack
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_SCOPE_H
#define JKCC_SCOPE_H


#include <stdint.h>

#include <jkcc/symbol.h>
#include <jkcc/vector.h>


#define SCOPE_NO_PUSH_IDENTIFIER (1 << 0)
#define SCOPE_NO_PUSH_TAG        (1 << 1)


typedef struct context_s {
	struct {
		uint_fast8_t  storage_class;
		ast_t        *type;
	} base;
	struct {
		symbol_table_t *identifier;
		symbol_table_t *tag;
		uint_fast8_t    storage_class;
		ast_t          *type;
	} current;
} context_t;

typedef struct scope_s {
	context_t context;
	vector_t  stack;    // context_t
	vector_t  history;  // context_t
} scope_t;


scope_t *scope_init(
	void);
void scope_free(
	scope_t      *scope);
void scope_pop(
	scope_t      *scope);
int scope_push(
	scope_t      *scope,
	uint_fast8_t  flags);


#endif  /* JCC_SCOPE_H */
