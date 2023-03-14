// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * main.h -- jkcc
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_PRIVATE_MAIN_H
#define JKCC_PRIVATE_MAIN_H


#include <argp.h>


#define KEY_COLOR 257
#define KEY_TRACE 258


static void    cleanup(void);
static error_t parse_opt(int key, char *arg, struct argp_state *state);


#endif  /* JKCC_PRIVATE_MAIN_H */
