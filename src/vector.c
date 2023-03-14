// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * vector.c -- dynamic size arrays
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/vector.h>

#include <stddef.h>
#include <stdlib.h>


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
