// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * string.c -- string type
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/string.h>
#include <jkcc/private/string.h>

#include <stddef.h>
#include <stdlib.h>
#include <string.h>


int string_append(string_t *string, const char *str, size_t len)
{
	if (!len) len = strlen(str);

	size_t used = string->tail - string->head + 1;

	if (string->size - used < len)
		if (resize(string, len) < 0) goto error;

	memcpy(string->tail, str, len);

	string->tail += len;

	*string->tail = '\0';

	return 0;

error:
	return -1;
}

void string_free(string_t *string)
{
	if (!string) return;

	free(string->head);
}

int string_init(string_t *string, size_t size)
{
	// ensure size is even
	size &= ~((size_t) 1);

	if (!size) size = STRING_DEFAULT_SIZE;

	char *tmp = calloc(size, sizeof(*tmp));
	if (!tmp) goto error;

	*tmp = '\0';
	string->head = tmp;
	string->tail = tmp;
	string->size = size;

	return 0;

error:
	return -1;
}


static int resize(string_t *string, size_t request)
{
	size_t used = string->tail - string->head + 1;

	size_t new_size;
	size_t prv_size;

	prv_size = new_size = string->size;

	do {
		new_size *= 2;

		// overflow
		if (new_size / 2 != prv_size) goto error;

		prv_size = new_size;
	} while (new_size - used < request);

	char *tmp = realloc(string->head, new_size);
	if (!tmp) goto error;

	string->head = tmp;
	string->tail = tmp + used - 1;
	string->size = new_size;

	return 0;

error:
	return -1;
}
