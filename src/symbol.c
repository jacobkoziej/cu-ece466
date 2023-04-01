// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * symbol.c -- symbol table
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/symbol.h>

#include <stddef.h>
#include <stdlib.h>

#include <jkcc/ast.h>
#include <jkcc/ht.h>


symbol_table_t *symbol_init(void)
{
	symbol_table_t *symbol = calloc(1, sizeof(*symbol));
	if (!symbol) return NULL;

	if (ht_init(&symbol->table, 0)) goto ht_init_error;

	if (vector_init(&symbol->table_insert_history, sizeof(ast_t*), 0))
		goto vector_init_error;

	return symbol;

vector_init_error:
	ht_free(&symbol->table, NULL);

ht_init_error:
	free(symbol);

	return NULL;
}

void symbol_free(symbol_table_t *symbol)
{
	if (!symbol) return;

	ht_free(&symbol->table, NULL);
	vector_free(&symbol->table_insert_history);
	free(symbol);
}
