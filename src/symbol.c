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


scope_t *scope_init(void)
{
	scope_t *scope = calloc(1, sizeof(*scope));
	if (!scope) return NULL;

	scope->current.identifier = symbol_init();
	if (!scope->current.identifier) goto error;

	if (vector_init(
		&scope->history.identifier,
		sizeof(scope->history.identifier),
		0)) goto error;
	if (vector_append(
		&scope->history.identifier,
		&scope->current.identifier)) goto error;

	return scope;

error:
	symbol_free(scope->current.identifier);

	vector_free(&scope->history.identifier);

	return NULL;
}

void scope_free(
	scope_t *scope)
{
	if (!scope) return;

	symbol_table_t **symbol;

	symbol = scope->history.identifier.buf;
	for (size_t i = 0; i < scope->history.identifier.use; i++)
		symbol_free(symbol[i]);

	vector_free(&scope->history.identifier);

	free(scope);
}

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
