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

	/*
	 * Array, pointer, and function AST types all use references to
	 * the same base type AST node.  We only want to free the types
	 * here which are unique to each identifier.  The freeing of the
	 * base type is handled by ast_declaration_free().
	 */
	ast_t **type = symbol->table_insert_history.buf;
	for (size_t i = 0; i < symbol->table_insert_history.use; i++)
		if (*type[i] == AST_POINTER || *type[i] == AST_ARRAY)
			AST_NODE_FREE(type[i]);

	vector_free(&symbol->table_insert_history);

	free(symbol);
}

int symbol_get(
	symbol_table_t  *symbol,
	const char      *key,
	size_t           len,
	ast_t          **type)
{
	void *ast;

	// search for symbol, climbing up scopes
	do {
		if (!ht_get(&symbol->table, key, len, &ast)) continue;

		if (type) *type = ast;

		return 0;
	} while (
		symbol = OFFSETOF_LIST(&symbol->list, symbol_table_t, list),
		symbol->list.prev);

	return SYMBOL_ERROR_NOT_FOUND;
}
