// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * ht.c -- hash table
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ht.h>
#include <jkcc/private/ht.h>

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>


void ht_free(ht_t *ht, void (*entry_free)(ht_entry_t *entry))
{
	if (!ht) return;

	if (entry_free)
		for (size_t i = 0; i < ht->size; i++)
			entry_free(&ht->entries[i]);

	free(ht->entries);
}

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


static uint64_t fnv1a_hash(const void *key, size_t size)
{
	uint64_t hash = FNV_OFFSET_BASIS;

	const unsigned char *byte = key;
	while (size--) {
		hash ^= *byte++;
		hash *=  FNV_PRIME;
	}

	return hash;
}
