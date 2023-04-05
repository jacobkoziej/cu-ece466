// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * location.h -- location types
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_LOCATION_H
#define JKCC_LOCATION_H


#include <stddef.h>
#include <sys/types.h>

#include <jkcc/list.h>


typedef struct file_s {
	char   *path;
	list_t  list;
	size_t  refs;
} file_t;

typedef struct location_s {
	file_t *file;
	struct {
		off_t offset;
		int   line;
		int   column;
	} start;
	struct {
		off_t offset;
		int   line;
		int   column;
	} end;
} location_t;


#endif  /* JKCC_LOCATION_H */
