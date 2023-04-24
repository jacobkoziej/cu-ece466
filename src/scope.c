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

	scope->context.current.tag = symbol_init();
	if (!scope->context.current.tag) goto error;

	if (vector_init(
		&scope->history.context,
		sizeof(scope->context),
		0)) goto error;
	if (vector_init(
		&scope->history.identifier,
		sizeof(scope->context.current.identifier),
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

	symbol = scope->history.tag.buf;
	for (size_t i = 0; i < scope->history.tag.use; i++)
		symbol_free(symbol[i]);

	vector_free(&scope->history.context);
	vector_free(&scope->history.identifier);
	vector_free(&scope->history.tag);

	free(scope);
}

void scope_pop(scope_t *scope)
{
	void *context = &scope->context;

	vector_pop(&scope->history.context, &context);
}

int scope_push(scope_t *scope)
{
	symbol_table_t *identifier;
	symbol_table_t *tag;

	identifier = NULL;
	tag        = NULL;

	identifier = symbol_init();
	if (!identifier) return -1;

	tag = symbol_init();
	if (!tag) goto symbol_tag_error;

	identifier->list.prev = &scope->context.current.identifier->list;
	tag->list.prev        = &scope->context.current.tag->list;

	if (vector_append(&scope->history.context, &scope->context))
		goto vector_context_error;
	if (vector_append(&scope->history.identifier, &identifier))
		goto vector_identifier_error;
	if (vector_append(&scope->history.tag, &tag))
		goto vector_tag_error;

	scope->context.current.identifier = identifier;
	scope->context.current.tag        = tag;

	return 0;

vector_tag_error:
	vector_pop(&scope->history.identifier, NULL);

vector_identifier_error:
	vector_pop(&scope->history.context, NULL);

vector_context_error:
	symbol_free(tag);

symbol_tag_error:
	symbol_free(identifier);

	return SYMBOL_ERROR_NOMEM;
}

int scope_push_identifier(scope_t *scope)
{
	symbol_table_t *identifier;

	identifier = symbol_init();
	if (!identifier) return -1;

	identifier->list.prev = &scope->context.current.identifier->list;

	if (vector_append(&scope->history.context, &scope->context))
		goto context_error;
	if (vector_append(&scope->history.identifier, &identifier))
		goto identifier_error;

	scope->context.current.identifier = identifier;

	return 0;

identifier_error:
	vector_pop(&scope->history.context, NULL);

context_error:
	symbol_free(identifier);

	return SYMBOL_ERROR_NOMEM;
}
