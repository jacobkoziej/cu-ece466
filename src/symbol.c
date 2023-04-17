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
#include <jkcc/list.h>
#include <jkcc/string.h>


symbol_table_t *symbol_init(void)
{
	symbol_table_t *symbol = calloc(1, sizeof(*symbol));
	if (!symbol) return NULL;

	if (ht_init(&symbol->table, 0)) goto ht_init_error;

	return symbol;

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

	++symbol->size;

	return 0;
}

void symbol_free(symbol_table_t *symbol)
{
	if (!symbol) return;

	ht_free(&symbol->table, NULL);

	free(symbol);
}

int symbol_get_identifier(
	symbol_table_t  *symbol,
	ast_t           *identifier,
	ast_t          **type)
{
	void           *ast;
	const string_t *key = ast_identifier_get_string(identifier);
	size_t          len = key->tail - key->head;

	// search for symbol, climbing up scopes
	do {
		if (!ht_get(&symbol->table, key->head, len, &ast)) continue;

		*type = ast;

		return 0;
	} while (
		symbol = OFFSETOF_LIST(&symbol->list, symbol_table_t, list),
		symbol->list.prev);

	return SYMBOL_ERROR_NOT_FOUND;
}

int symbol_insert_identifier(
	symbol_table_t *symbol,
	ast_t          *identifier,
	ast_t          *type)
{
	ast_identifier_set_type(identifier, type);
	const string_t *key = ast_identifier_get_string(identifier);

	return symbol_insert(symbol, key->head, key->tail - key->head, type);
}
