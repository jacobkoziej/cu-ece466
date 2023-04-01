// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * symbol.h -- symbol table
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_SYMBOL_H
#define JKCC_SYMBOL_H


#include <jkcc/ht.h>
#include <jkcc/list.h>


typedef struct symbol_table_s {
	ht_t   table;
	list_t list;
} symbol_table_t;


symbol_table_t *symbol_init(void);


#endif  /* JCC_SYMBOL_H */
