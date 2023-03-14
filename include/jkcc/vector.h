// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * vector.h -- dynamic size arrays
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_VECTOR_H
#define JKCC_VECTOR_H


#include <stddef.h>


typedef struct vector_s {
	void   *buf;
	size_t  use;
	size_t  size;
	size_t  element_size;
} vector_t;


#endif  /* JCC_VECTOR_H */
