// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * vector.h -- dynamic size arrays
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_VECTOR_H
#define JKCC_VECTOR_H


#include <stddef.h>


#define VECTOR_DEFAULT_SIZE 64


typedef struct vector_s {
	void   *buf;
	size_t  use;
	size_t  size;
	size_t  element_size;
} vector_t;


int  vector_append(vector_t *vector, void *element);
void vector_free(vector_t *vector);
int  vector_init(vector_t *vector, size_t element_size, size_t size);
void vector_pop(vector_t *vector, void **element);
int  vector_resize(vector_t *vector, size_t size);


#endif  /* JCC_VECTOR_H */
