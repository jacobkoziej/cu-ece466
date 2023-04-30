// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * symbol.h -- symbol table
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_SYMBOL_H
#define JKCC_SYMBOL_H


#include <stdbool.h>
#include <stddef.h>

#include <jkcc/ast/ast.h>
#include <jkcc/ht.h>
#include <jkcc/list.h>


#define SYMBOL_ERROR_NOMEM     (-1)
#define SYMBOL_ERROR_EXISTS    (-2)
#define SYMBOL_ERROR_NOT_FOUND (-3)


typedef struct symbol_table_s {
	ht_t   table;
	size_t size;
	list_t list;
} symbol_table_t;


int symbol_check_identifier_collision(
	symbol_table_t  *symbol,
	ast_t           *identifier);
symbol_table_t *symbol_init(
	void);
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
int symbol_insert_tag(
	symbol_table_t  *symbol,
	ast_t           *tag,
	bool             struct_definition,
	ast_t           *type);


#endif  /* JCC_SYMBOL_H */
