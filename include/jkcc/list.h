// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * list.h -- linked list
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_LIST_H
#define JKCC_LIST_H


typedef struct list_s {
	struct list_s *prev;
	struct list_s *next;
} list_t;


#endif  /* JCC_LIST_H */
