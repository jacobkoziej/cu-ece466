// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * ansi-esc.h -- ANSI escape sequences
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_ANSI_ESC
#define JKCC_ANSI_ESC


#define ANSI_CSI "\033["

#define ANSI_SGR "m"

#define ANSI_SGR_RESET     "0"
#define ANSI_SGR_BOLD      "1"
#define ANSI_SGR_LIGHT     "2"
#define ANSI_SGR_ITALIC    "3"
#define ANSI_SGR_UNDERLINE "4"

#define ANSI_SGR_FOREGROUND        "3"
#define ANSI_SGR_BACKGROUND        "4"
#define ANSI_SGR_BRIGHT_FOREGROUND "9"
#define ANSI_SGR_BRIGHT_BACKGROUND "10"

#define ANSI_SGR_BLACK   "0"
#define ANSI_SGR_RED     "1"
#define ANSI_SGR_GREEN   "2"
#define ANSI_SGR_YELLOW  "3"
#define ANSI_SGR_BLUE    "4"
#define ANSI_SGR_MAGENTA "5"
#define ANSI_SGR_CYAN    "6"
#define ANSI_SGR_WHITE   "7"


#endif  /* JKCC_ANSI_ESC */
