// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * scope.c -- scope stack
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/scope.h>

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <jkcc/symbol.h>
#include <jkcc/vector.h>


scope_t *scope_init(void)
{
	scope_t *scope = calloc(1, sizeof(*scope));
	if (!scope) return NULL;

	scope->context.current.identifier = symbol_init();
	if (!scope->context.current.identifier) goto error;

	if (vector_init(
		&scope->history.context,
		sizeof(scope->context),
		0)) goto error;
	if (vector_init(
		&scope->history.identifier,
		sizeof(scope->context.current.identifier),
		0)) goto error;
	if (vector_append(
		&scope->history.identifier,
		&scope->context.current.identifier)) goto error;

	return scope;

error:
	symbol_free(scope->context.current.identifier);

	vector_free(&scope->history.context);
	vector_free(&scope->history.identifier);

	return NULL;
}

void scope_free(scope_t *scope)
{
	if (!scope) return;

	symbol_table_t **symbol;

	symbol = scope->history.identifier.buf;
	for (size_t i = 0; i < scope->history.identifier.use; i++)
		symbol_free(symbol[i]);

	vector_free(&scope->history.context);
	vector_free(&scope->history.identifier);

	free(scope);
}

int scope_push(scope_t *scope)
{
	symbol_table_t *symbol;

	symbol = symbol_init();
	if (!symbol) return -1;

	symbol->list.prev = &scope->context.current.identifier->list;

	if (vector_append(&scope->history.context, &scope->context))
		goto context_error;
	if (vector_append(&scope->history.identifier, &symbol))
		goto identifier_error;

	memset(&scope->context, 0, sizeof(scope->context));
	scope->context.current.identifier = symbol;

	return 0;

identifier_error:
	vector_pop(&scope->history.context, NULL);

context_error:
	symbol_free(symbol);

	return SYMBOL_ERROR_NOMEM;
}
