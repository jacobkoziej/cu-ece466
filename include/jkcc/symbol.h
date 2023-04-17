// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * symbol.h -- symbol table
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_SYMBOL_H
#define JKCC_SYMBOL_H


#include <stddef.h>

#include <jkcc/ast.h>
#include <jkcc/ht.h>
#include <jkcc/list.h>
#include <jkcc/vector.h>


#define SYMBOL_ERROR_NOMEM     (-1)
#define SYMBOL_ERROR_EXISTS    (-2)
#define SYMBOL_ERROR_NOT_FOUND (-3)


typedef struct symbol_table_s {
	ht_t   table;
	size_t size;
	list_t list;
} symbol_table_t;

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
	scope_t         *scope);
symbol_table_t *symbol_init(
	void);
int symbol_insert(
	symbol_table_t  *symbol,
	const char      *identifier,
	size_t           len,
	ast_t           *type);
void symbol_free(
	symbol_table_t  *symbol);
int symbol_get_identifier(
	symbol_table_t  *symbol,
	ast_t           *identifier,
	ast_t          **type);
int symbol_insert_identifier(
	symbol_table_t  *symbol,
	ast_t           *identifier,
	ast_t           *type);


#endif  /* JCC_SYMBOL_H */
