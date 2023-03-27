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

	ht->entries = calloc(ht->size, sizeof(*ht->entries));
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

static int rehash(ht_t *ht)
{
	size_t size = ht->size * 2;

	// overflow
	if (size / 2 != ht->size) return -1;

	ht_entry_t *new_entries = calloc(size, sizeof(*new_entries));
	if (!new_entries) return -1;

	size_t use = ht->use;

	ht_entry_t *entry;

	for (size_t i = 0; i < ht->size; i++) {
		if (!use) break;

		entry = &ht->entries[i];

		if (!entry->key) continue;
		if (entry->attributes & HT_ATTRIBUTE_DELETED) {
			free(entry->key);
			continue;
		}

		uint64_t pos = fnv1a_hash(entry->key, entry->size) % size;

		// potentially dangerous infinite loop ;)
		for (;;) {
			ht_entry_t *tmp = &new_entries[pos];
			pos = (pos + 1) % size;

			// probe over set entries
			if (tmp->key) continue;

			*tmp = *entry;
			break;
		}

		--use;
	}

	free(ht->entries);

	ht->entries = new_entries;
	ht->size    = size;

	return 0;
}
