// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * ht.h -- hash table
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_HT_H
#define JKCC_HT_H


#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


#define HT_ATTRIBUTE_DELETED (1 << 0)

#define HT_DEFAULT_SIZE 64


typedef struct ht_entry_s {
	void         *val;
	void         *key;
	size_t        size;
	uint_fast8_t  attributes;
} ht_entry_t;

typedef struct ht_s {
	ht_entry_t *entries;
	size_t      use;
	size_t      size;
} ht_t;


bool ht_exists(ht_t *ht, const void *key, size_t size);
void ht_free(ht_t *ht, void (*entry_free)(ht_entry_t *entry));
int  ht_init(ht_t *ht, size_t size);


#endif  /* JCC_HT_H */
