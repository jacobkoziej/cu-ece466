// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * ht.h -- hash table
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_PRIVATE_HT_H
#define JKCC_PRIVATE_HT_H


#include <jkcc/ht.h>

#include <stdint.h>


#define FNV_OFFSET_BASIS 0xcbf29ce484222325UL
#define FNV_PRIME        0x100000001b3UL


static uint64_t fnv1a_hash(const void *key, size_t size);
static int      rehash(ht_t *ht);


#endif  /* JCC_PRIVATE_HT_H */
