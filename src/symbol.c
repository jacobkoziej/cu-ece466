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

int symbol_insert(
	symbol_table_t *symbol,
	const char     *key,
	size_t          len,
	ast_t          *type)
{
	if (ht_exists(&symbol->table, key, len))
		return SYMBOL_ERROR_EXISTS;

	if (ht_insert(&symbol->table, key, len, type))
		return SYMBOL_ERROR_NOMEM;

	if (vector_append(&symbol->table_insert_history, &type))
		goto vector_append_error;

	++symbol->size;

	return 0;

vector_append_error:
	ht_rm(&symbol->table, key, len, NULL);

	return SYMBOL_ERROR_NOMEM;
}

void symbol_free(symbol_table_t *symbol)
{
	if (!symbol) return;

	ht_free(&symbol->table, NULL);
	vector_free(&symbol->table_insert_history);
	free(symbol);
}
