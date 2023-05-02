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
	if (vector_init(
		&scope->history.identifier,
		sizeof(scope->context.current.identifier),
		0)) goto error;
	if (vector_init(
		&scope->history.label,
		sizeof(scope->context.current.label),
		0)) goto error;
	if (vector_init(
		&scope->history.tag,
		sizeof(scope->context.current.tag),
		0)) goto error;

	if (vector_append(
		&scope->history.identifier,
		&scope->context.current.identifier)) goto error;
	if (vector_append(
		&scope->history.tag,
		&scope->context.current.tag)) goto error;

	return scope;

error:
	symbol_free(scope->context.current.identifier);
	symbol_free(scope->context.current.tag);

	vector_free(&scope->history.identifier);
	vector_free(&scope->history.label);
	vector_free(&scope->history.tag);
	vector_free(&scope->stack);

	return NULL;
}

void scope_free(scope_t *scope)
{
	if (!scope) return;

	symbol_table_t **symbol;

	symbol = scope->history.identifier.buf;
	for (size_t i = 0; i < scope->history.identifier.use; i++)
		symbol_free(symbol[i]);

	symbol = scope->history.label.buf;
	for (size_t i = 0; i < scope->history.label.use; i++)
		symbol_free(symbol[i]);

	symbol = scope->history.tag.buf;
	for (size_t i = 0; i < scope->history.tag.use; i++)
		symbol_free(symbol[i]);

	vector_free(&scope->stack);
	vector_free(&scope->history.identifier);
	vector_free(&scope->history.label);
	vector_free(&scope->history.tag);

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
	symbol_table_t *label      = NULL;
	symbol_table_t *tag        = NULL;

	if (vector_append(&scope->stack, &scope->context))
		goto error_vector_append_stack;

	if (!(flags & SCOPE_NO_PUSH_IDENTIFIER)) {
		identifier = symbol_init();
		if (!identifier) goto error_symbol_init_identifier;

		if (vector_append(&scope->history.identifier, &identifier))
			goto error_vector_append_history_identifier;

		scope->context.current.identifier = identifier;
		scope->context.current.type_stack = 0;
	}

	if (!(flags & SCOPE_NO_PUSH_LABEL)) {
		label = symbol_init();
		if (!label) goto error_symbol_init_label;

		if (vector_append(&scope->history.label, &label))
			goto error_vector_append_history_label;

		scope->context.current.label = label;
	}

	if (!(flags & SCOPE_NO_PUSH_TAG)) {
		tag = symbol_init();
		if (!tag) goto error_symbol_init_tag;

		if (vector_append(&scope->history.tag, &tag))
			goto error_vector_append_history_tag;

		scope->context.current.tag = tag;
	}

	return 0;

error_vector_append_history_tag:
	symbol_free(tag);

error_symbol_init_tag:
	if (label) vector_pop(&scope->history.label, NULL);

error_vector_append_history_label:
	symbol_free(label);

error_symbol_init_label:
	if (identifier) vector_pop(&scope->history.identifier, NULL);

error_vector_append_history_identifier:
	symbol_free(identifier);

error_symbol_init_identifier:;
	void *element = &scope->context;
	vector_pop(&scope->stack, &element);

error_vector_append_stack:
	return -1;
}
