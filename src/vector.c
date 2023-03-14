// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * vector.c -- dynamic size arrays
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/vector.h>

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


int vector_append(vector_t *vector, void *element)
{
	if (vector->use + 1 > vector->size) {
		size_t new_size = vector->size * 2;

		// overflow
		if (new_size / 2 != vector->size) return -1;

		if (vector_resize(vector, new_size)) return -1;
	}

	void *dst = (void*) (((uintptr_t) vector->buf) +
		(vector->use * vector->element_size));

	memcpy(dst, element, vector->element_size);

	++vector->use;

	return 0;
}

void vector_free(vector_t *vector)
{
	free(vector->buf);
}

int vector_init(vector_t *vector, size_t element_size, size_t size)
{
	if (!element_size) return -1;

	if (!size) size = VECTOR_DEFAULT_SIZE;

	vector->use          = 0;
	vector->size         = size;
	vector->element_size = element_size;

	vector->buf = malloc(vector->size * vector->element_size);
	if (!vector->buf) return -1;

	return 0;
}

int vector_resize(vector_t *vector, size_t size)
{
	vector_t *tmp = realloc(vector->buf, size * vector->element_size);
	if (!tmp) return -1;

	vector->buf  = tmp;
	vector->size = size;

	return 0;
}
