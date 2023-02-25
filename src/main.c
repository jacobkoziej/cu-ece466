// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * main.c -- jkcc
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <argp.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <jkcc/jkcc.h>

#include "vcs-tag.h"


#define KEY_COLOR 257


static error_t parse_opt(int key, char *arg, struct argp_state *state);


const char *argp_program_version     = VCS_TAG;
const char *argp_program_bug_address = "<jacobkoziej@gmail.com>";

static const struct argp_option options[] = {
	{
		.name  = "color",
		.key   = KEY_COLOR,
		.arg   = "WHEN",
		.doc   = "Override color output;\nWHEN is 'stdout', 'stderr', 'always', or 'never'"
	},
	{0},
};

static const struct argp argp = {
	.options  = options,
	.parser   = parse_opt,
	.args_doc = "[FILE]...",
};


static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
	jkcc_t *jkcc = state->input;

	switch (key) {
		case ARGP_KEY_ARGS:
			jkcc->file = (const char**) state->argv + state->next;
			jkcc->file_count = state->argc - state->next;

			// let argp_parse() know we've consumed
			// all remaining arguments
			state->next = state->argc;
			break;

		case KEY_COLOR:
			if (!strcmp(arg, "stdout")) {
				jkcc->config.ansi_sgr_stdout = 1;
				jkcc->config.ansi_sgr_stderr = 0;
				break;
			}

			if (!strcmp(arg, "stderr")) {
				jkcc->config.ansi_sgr_stdout = 0;
				jkcc->config.ansi_sgr_stderr = 1;
				break;
			}

			if (!strcmp(arg, "always")) {
				jkcc->config.ansi_sgr_stdout = 1;
				jkcc->config.ansi_sgr_stderr = 1;
				break;
			}

			if (!strcmp(arg, "never")) {
				jkcc->config.ansi_sgr_stdout = 0;
				jkcc->config.ansi_sgr_stderr = 0;
				break;
			}

			argp_error(
				state,
				"unrecognized argument: '%s'",
				arg);

			break;

		default:
			return ARGP_ERR_UNKNOWN;
	}

	return 0;
}


int main(int argc, char **argv)
{
	static jkcc_t jkcc;

	// automatic config
	jkcc.config.ansi_sgr_stdout = isatty(STDOUT_FILENO);
	jkcc.config.ansi_sgr_stderr = isatty(STDERR_FILENO);

	argp_parse(&argp, argc, argv, 0, 0, &jkcc);

	return EXIT_SUCCESS;
}
