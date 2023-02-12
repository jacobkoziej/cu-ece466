// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * string.h -- string type
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_STRING_H
#define JKCC_STRING_H


#include <stddef.h>


typedef struct string_s {
	char   *head;
	char   *tail;
	size_t  size;
} string_t;


int string_append(string_t *string, const char *str, size_t len);


#endif  /* JKCC_STRING_H */
