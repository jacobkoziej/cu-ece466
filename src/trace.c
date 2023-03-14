// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * trace.c -- trace program execution
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/trace.h>
#include <jkcc/private/trace.h>

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <jkcc/ansi-esc.h>
#include <jkcc/string.h>


void trace_args(
	trace_t    *trace,
	const char *file,
	const char *func,
	const char *args,
	const char *format,
	...)
{
	string_t fmt;

	char *wformat = NULL;
	char *wargs   = NULL;

	if (string_init(&fmt, 0) < 0) goto error;

	wformat = strdup(format);
	if (!wformat) goto error;

	wargs = strdup(args);
	if (!wargs) goto error;

	char *format_saveptr;
	char *args_saveptr;

	char *token;

	token = strtok_r(wformat, TRACE_ARGS_DELIM, &format_saveptr);
	if (string_append(&fmt,  token, 0) < 0) goto error;
	if (string_append(&fmt, " == ", 0) < 0) goto error;
	token = strtok_r(wargs, TRACE_ARGS_DELIM, &args_saveptr);
	if (string_append(&fmt,  token, 0) < 0) goto error;

	while ((token = strtok_r(NULL, TRACE_ARGS_DELIM, &format_saveptr))) {
		if (string_append(&fmt,   ", ", 0) < 0) goto error;
		if (string_append(&fmt,  token, 0) < 0) goto error;
		if (string_append(&fmt, " == ", 0) < 0) goto error;
		token = strtok_r(NULL, TRACE_ARGS_DELIM, &args_saveptr);
		if (string_append(&fmt,  token, 0) < 0) goto error;
	}

	if (trace->level >= JKCC_TRACE_LEVEL_HIGH)
		print_time(trace);

	if (trace->ansi_sgr)
		fprintf(
			trace->stream,
			ANSI_CSI
			ANSI_SGR_BOLD
			";"
			ANSI_SGR_FOREGROUND
			ANSI_SGR_GREEN
			ANSI_SGR
			"%s"
			ANSI_CSI
			ANSI_SGR_FOREGROUND
			ANSI_SGR_WHITE
			ANSI_SGR
			":"
			ANSI_CSI
			ANSI_SGR_FOREGROUND
			ANSI_SGR_CYAN
			ANSI_SGR
			"%s"
			ANSI_CSI
			ANSI_SGR_FOREGROUND
			ANSI_SGR_WHITE
			ANSI_SGR
			"("
			ANSI_CSI
			ANSI_SGR_RESET
			ANSI_SGR,
			file,
			func);
	else
		fprintf(trace->stream, "%s:%s(", file, func);

	va_list va;
	va_start(va, format);

	vfprintf(trace->stream, fmt.head, va);

	va_end(va);

	if (trace->ansi_sgr)
		fprintf(
			trace->stream,
			ANSI_CSI
			ANSI_SGR_BOLD
			";"
			ANSI_SGR_FOREGROUND
			ANSI_SGR_WHITE
			ANSI_SGR
			")"
			ANSI_CSI
			ANSI_SGR_RESET
			ANSI_SGR
			"\n");
	else
		fprintf(trace->stream, ")\n");

	free(wformat);
	free(wargs);

	string_free(&fmt);

	return;

error:
	free(wformat);
	free(wargs);

	string_free(&fmt);

	fprintf(trace->stream, "trace_args() failed\n");
}

void trace_printf(
	trace_t    *trace,
	const char *file,
	const char *func,
	const int   line,
	const char *format,
	...)
{
	if (trace->level >= JKCC_TRACE_LEVEL_HIGH)
		print_time(trace);

	if (trace->ansi_sgr)
		fprintf(
			trace->stream,
			ANSI_CSI
			ANSI_SGR_BOLD
			";"
			ANSI_SGR_FOREGROUND
			ANSI_SGR_GREEN
			ANSI_SGR
			"%s"
			ANSI_CSI
			ANSI_SGR_FOREGROUND
			ANSI_SGR_WHITE
			ANSI_SGR
			":"
			ANSI_CSI
			ANSI_SGR_FOREGROUND
			ANSI_SGR_CYAN
			ANSI_SGR
			"%s"
			ANSI_CSI
			ANSI_SGR_FOREGROUND
			ANSI_SGR_WHITE
			ANSI_SGR
			":"
			ANSI_CSI
			ANSI_SGR_FOREGROUND
			ANSI_SGR_MAGENTA
			ANSI_SGR
			"%d"
			ANSI_CSI
			ANSI_SGR_FOREGROUND
			ANSI_SGR_WHITE
			ANSI_SGR
			": "
			ANSI_CSI
			ANSI_SGR_RESET
			ANSI_SGR,
			file,
			func,
			line);
	else
		fprintf(trace->stream, "%s:%s:%d: ", file, func, line);

	va_list ap;
	va_start(ap, format);

	vfprintf(trace->stream, format, ap);

	va_end(ap);

	fprintf(trace->stream, "\n");
}

void trace_rule(
	trace_t    *trace,
	const char *file,
	const char *func,
	const char *rule,
	const char *match)
{
	if (trace->level >= JKCC_TRACE_LEVEL_HIGH)
		print_time(trace);

	if (trace->ansi_sgr)
		fprintf(
			trace->stream,
			ANSI_CSI
			ANSI_SGR_BOLD
			";"
			ANSI_SGR_FOREGROUND
			ANSI_SGR_GREEN
			ANSI_SGR
			"%s"
			ANSI_CSI
			ANSI_SGR_FOREGROUND
			ANSI_SGR_WHITE
			ANSI_SGR
			":"
			ANSI_CSI
			ANSI_SGR_FOREGROUND
			ANSI_SGR_CYAN
			ANSI_SGR
			"%s"
			ANSI_CSI
			ANSI_SGR_FOREGROUND
			ANSI_SGR_WHITE
			ANSI_SGR
			": "
			ANSI_CSI
			ANSI_SGR_RESET
			ANSI_SGR
			ANSI_CSI
			ANSI_SGR_FOREGROUND
			ANSI_SGR_MAGENTA
			ANSI_SGR
			"%s"
			ANSI_CSI
			ANSI_SGR_FOREGROUND
			ANSI_SGR_WHITE
			ANSI_SGR
			": "
			ANSI_CSI
			ANSI_SGR_FOREGROUND
			ANSI_SGR_BLUE
			ANSI_SGR
			"%s\n"
			ANSI_CSI
			ANSI_SGR_RESET
			ANSI_SGR
			ANSI_CSI,
			file,
			func,
			rule,
			match);
	else
		fprintf(
			trace->stream,
			"%s:%s: %s: %s\n",
			file,
			func,
			rule,
			match);
}


static void print_time(trace_t *trace)
{
	time_t now = time(NULL);

	struct tm tm;
	localtime_r(&now, &tm);

	char buf[sizeof(ISO_8601_2000_FORMAT)];
	strftime(buf, sizeof(buf), "%FT%T%z", &tm);

	if (trace->ansi_sgr)
		fprintf(
			trace->stream,
			ANSI_CSI
			ANSI_SGR_BOLD
			";"
			ANSI_SGR_BRIGHT_FOREGROUND
			ANSI_SGR_BLACK
			ANSI_SGR
			"%s"
			ANSI_CSI
			ANSI_SGR_FOREGROUND
			ANSI_SGR_WHITE
			ANSI_SGR
			":"
			ANSI_CSI
			ANSI_SGR_RESET
			ANSI_SGR,
			buf);
	else
		fprintf(trace->stream, "%s:", buf);
}
