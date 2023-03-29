// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * trace.h -- trace program execution
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_PRIVATE_TRACE_H
#define JKCC_PRIVATE_TRACE_H


#include <jkcc/trace.h>


#define ISO_8601_2000_FORMAT "yyyy-hh-mmThh:mm:ss-hhmm"
#define TRACE_ARGS_DELIM     " ,\t\n"


static void print_time(trace_t *trace);


#endif  /* JKCC_PRIVATE_TRACE_H */
