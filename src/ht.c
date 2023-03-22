// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * ht.c -- hash table
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ht.h>

#include <stddef.h>
#include <stdlib.h>


int ht_init(ht_t *ht, size_t size)
{
	// ensure size is even
	size &= ~((size_t) 1);

	if (!size) size = HT_DEFAULT_SIZE;

	ht->use  = 0;
	ht->size = size;

	ht->entries = malloc(ht->size * sizeof(*ht->entries));
	if (!ht->entries) return -1;

	return 0;
}
