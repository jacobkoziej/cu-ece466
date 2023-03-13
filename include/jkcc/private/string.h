// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * string.h -- string type
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_PRIVATE_STRING_H
#define JKCC_PRIVATE_STRING_H


#include <jkcc/string.h>

#include <stddef.h>


static int resize(string_t *string, size_t request);


#endif  /* JKCC_PRIVATE_STRING_H */
