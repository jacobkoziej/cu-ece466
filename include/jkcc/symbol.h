// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * symbol.h -- symbol table
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_SYMBOL_H
#define JKCC_SYMBOL_H


#include <jkcc/ht.h>
#include <jkcc/list.h>
#include <jkcc/vector.h>


typedef struct symbol_table_s {
	ht_t     table;
	vector_t table_insert_history;  // ast_t*
	list_t   list;
} symbol_table_t;


symbol_table_t *symbol_init(void);
void            symbol_free(symbol_table_t *symbol);


#endif  /* JCC_SYMBOL_H */
