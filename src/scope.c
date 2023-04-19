// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * scope.c -- scope stack
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/scope.h>

#include <stddef.h>
#include <stdlib.h>

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
