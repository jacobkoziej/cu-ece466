// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * ht.c -- hash table
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ht.h>
#include <jkcc/private/ht.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


bool ht_exists(ht_t *ht, const void *key, size_t size)
{
	// invalid key size
	if (!size) return false;

	// empty hash table
	if (!ht->use) return false;

	uint64_t pos = fnv1a_hash(key, size) % ht->size;

	for (size_t i = 0; i < ht->size; pos = (pos + 1) % ht->size, i++) {
		ht_entry_t *entry = &ht->entries[pos];

		// we've passed the set entries
		if (!entry->key && !(entry->attributes & HT_ATTRIBUTE_DELETED))
			return false;

		if (entry->attributes & HT_ATTRIBUTE_DELETED) continue;
		if (entry->size != size) continue;

		if (!memcmp(entry->key, key, size))
			return true;
	}

	// we've somehow traversed the entire hash table
	return false;
}

void ht_free(ht_t *ht, void (*entry_free)(void *val))
{
	if (!ht) return;

	if (entry_free)
		for (size_t i = 0; i < ht->size; i++) {
			if (!ht->entries[i].key) continue;

			free(ht->entries[i].key);
			entry_free(&ht->entries[i].val);

			if (!--ht->use) break;
		}
	else
		for (size_t i = 0; i < ht->size; i++) {
			if (!ht->entries[i].key) continue;

			free(ht->entries[i].key);

			if (!--ht->use) break;
		}

	free(ht->entries);
}

int ht_get(ht_t *ht, const void *key, size_t size, void **val)
{
	// invalid key size
	if (!size) return -1;

	// empty hash table
	if (!ht->use) return -1;

	uint64_t pos = fnv1a_hash(key, size) % ht->size;

	for (size_t i = 0; i < ht->size; pos = (pos + 1) % ht->size, i++) {
		ht_entry_t *entry = &ht->entries[pos];

		// we've passed the set entries
		if (!entry->key && !(entry->attributes & HT_ATTRIBUTE_DELETED))
			return -1;

		if (entry->attributes & HT_ATTRIBUTE_DELETED) continue;
		if (entry->size != size) continue;

		if (!memcmp(entry->key, key, size)) {
			*val = entry->val;
			return 0;
		}
	}

	// we've somehow traversed the entire hash table
	return -1;
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

int ht_insert(ht_t *ht, const void *key, size_t size, void *val)
{
	// invalid key size
	if (!size) return -1;

	if (ht->use >= ht->size / 2)
		if (rehash(ht)) return -1;

	void *key_buf = malloc(size);
	if (!key_buf) return -1;

	uint64_t pos = fnv1a_hash(key, size) % ht->size;

	for (size_t i = 0; i < ht->size; pos = (pos + 1) % ht->size, i++) {
		ht_entry_t *entry = &ht->entries[pos];

		if (entry->key) {
			if (!(entry->attributes & HT_ATTRIBUTE_DELETED)) {
				// duplicate entry
				if (entry->size == size)
					if (!memcmp(entry->key, key, size))
						goto error;

				continue;
			}

			free(entry->key);
			entry->attributes &= ~HT_ATTRIBUTE_DELETED;
		}

		memcpy(entry->key = key_buf, key, size);

		entry->val  = val;
		entry->size = size;

		++ht->use;

		return 0;
	}

error:
	free(key_buf);

	// we've somehow traversed the entire hash table
	return -1;
}

int ht_rm(ht_t *ht, const void *key, size_t size, void **val)
{
	// invalid key size
	if (!size) return -1;

	// empty hash table
	if (!ht->use) return -1;

	uint64_t pos = fnv1a_hash(key, size) % ht->size;

	for (size_t i = 0; i < ht->size; pos = (pos + 1) % ht->size, i++) {
		ht_entry_t *entry = &ht->entries[pos];

		// we've passed the set entries
		if (!entry->key && !(entry->attributes & HT_ATTRIBUTE_DELETED))
			return -1;

		if (entry->size != size) continue;

		if (!memcmp(entry->key, key, size)) {
			if (val) *val = entry->val;

			entry->attributes |= HT_ATTRIBUTE_DELETED;
			--ht->use;

			return 0;
		}
	}

	// we've somehow traversed the entire hash table
	return -1;
}

int ht_set(ht_t *ht, const void *key, size_t size, void *val)
{
	// invalid key size
	if (!size) return -1;

	// empty hash table
	if (!ht->use) return -1;

	uint64_t pos = fnv1a_hash(key, size) % ht->size;

	for (size_t i = 0; i < ht->size; pos = (pos + 1) % ht->size, i++) {
		ht_entry_t *entry = &ht->entries[pos];

		// we've passed the set entries
		if (!entry->key && !(entry->attributes & HT_ATTRIBUTE_DELETED))
			return -1;

		if (entry->attributes & HT_ATTRIBUTE_DELETED) continue;
		if (entry->size != size) continue;

		if (!memcmp(entry->key, key, size)) {
			entry->val = val;
			return 0;
		}
	}

	// we've somehow traversed the entire hash table
	return -1;
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
