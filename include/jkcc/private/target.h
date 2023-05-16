// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * target.h -- garbage target code generation
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_PRIVATE_TARGET_H
#define JKCC_PRIVATE_TARGET_H


#define DIRECTIVE(directive) fprintf(stream, "\t" directive "\n")

#define DIRECTIVE_ARG(directive, ...) fprintf(stream, "\t" directive "\n", __VA_ARGS__)

#define LABEL(label) fprintf(stream, "%s:\n", label)

#define LOCAL_LABEL(bb) fprintf(stream, ".L%lu:\n", bb)


#endif  /* JKCC_PRIVATE_TARGET_H */
