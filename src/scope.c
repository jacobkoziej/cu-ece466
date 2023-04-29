// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * scope.c -- scope stack
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/scope.h>

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <jkcc/symbol.h>
#include <jkcc/vector.h>


scope_t *scope_init(void)
{
	scope_t *scope = calloc(1, sizeof(*scope));
	if (!scope) return NULL;

	scope->context.current.identifier = symbol_init();
	if (!scope->context.current.identifier) goto error;

	scope->context.current.tag = symbol_init();
	if (!scope->context.current.tag) goto error;

	if (vector_init(&scope->stack, sizeof(scope->context), 0))
		goto error;
	if (vector_init(&scope->history, sizeof(scope->context), 0))
		goto error;

	if (vector_append(&scope->history, &scope->context)) goto error;

	return scope;

error:
	symbol_free(scope->context.current.identifier);
	symbol_free(scope->context.current.tag);

	vector_free(&scope->history);
	vector_free(&scope->stack);

	return NULL;
}

void scope_free(scope_t *scope)
{
	if (!scope) return;

	// used to prevent double free()
	symbol_table_t *prv_identifier = NULL;
	symbol_table_t *prv_tag        = NULL;

	context_t *context = scope->history.buf;

	for (size_t i = 0; i < scope->history.use; i++) {
		if (prv_identifier != context[i].current.identifier) {
			symbol_free(context[i].current.identifier);
			prv_identifier = context[i].current.identifier;
		}

		if (prv_tag != context[i].current.tag) {
			symbol_free(context[i].current.tag);
			prv_tag = context[i].current.tag;
		}
	}

	vector_free(&scope->stack);
	vector_free(&scope->history);

	free(scope);
}

void scope_pop(scope_t *scope)
{
	void *context = &scope->context;

	vector_pop(&scope->stack, &context);
}

int scope_push(scope_t *scope, uint_fast8_t flags)
{
	symbol_table_t *identifier = NULL;
	symbol_table_t *tag        = NULL;

	if (vector_append(&scope->stack, &scope->context))
		goto error_vector_append_stack;

	if (!(flags & SCOPE_NO_PUSH_IDENTIFIER)) {
		identifier = symbol_init();
		if (!identifier) goto error_symbol_init_identifier;

		scope->context.current.identifier = identifier;
	}

	if (!(flags & SCOPE_NO_PUSH_TAG)) {
		tag = symbol_init();
		if (!tag) goto error_symbol_init_tag;

		scope->context.current.tag = tag;
	}

	if (vector_append(&scope->history, &scope->context))
		goto error_vector_append_history;

	return 0;

error_vector_append_history:
	symbol_free(tag);

error_symbol_init_tag:
	symbol_free(identifier);

error_symbol_init_identifier:;
	void *element = &scope->context;
	vector_pop(&scope->stack, &element);

error_vector_append_stack:
	return -1;
}
