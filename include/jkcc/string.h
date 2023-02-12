// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * string.h -- string type
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_STRING_H
#define JKCC_STRING_H


#include <stddef.h>


#define STRING_DEFAULT_SIZE 64


typedef struct string_s {
	char   *head;
	char   *tail;
	size_t  size;
} string_t;


int string_append(string_t *string, const char *str, size_t len);
int string_init(string_t *string, size_t size);


#endif  /* JKCC_STRING_H */
