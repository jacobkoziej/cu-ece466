// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * trace.c -- trace program execution
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/trace.h>

#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#include <jkcc/ansi-esc.h>


void trace_printf(
	trace_t    *trace,
	const char *file,
	const char *func,
	const int   line,
	const char *format,
	...)
{
	if (trace->ansi_sgr)
		fprintf(trace->stream, ANSI_CSI ANSI_SGR_BOLD ANSI_SGR);

	if (trace->level >= JKCC_TRACE_LEVEL_HIGH) {
		time_t now = time(NULL);

		struct tm tm;
		localtime_r(&now, &tm);

		char buf[sizeof("yyyy-hh-mmThh:mm:ss-hhmm")];
		strftime(buf, sizeof(buf), "%FT%T%z", &tm);

		if (trace->ansi_sgr)
			fprintf(
				trace->stream,
				ANSI_CSI
				ANSI_SGR_BRIGHT_FOREGROUND
				ANSI_SGR_BLACK
				ANSI_SGR
				"%s"
				ANSI_CSI
				ANSI_SGR_FOREGROUND
				ANSI_SGR_WHITE
				ANSI_SGR
				":",
				buf);
		else
			fprintf(trace->stream, "%s:", buf);
	}

	if (trace->ansi_sgr)
		fprintf(
			trace->stream,
			ANSI_CSI
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
			": ",
			file,
			func,
			line);
	else
		fprintf(trace->stream, "%s:%s:%d: ", file, func, line);


	if (trace->ansi_sgr)
		fprintf(trace->stream, ANSI_CSI ANSI_SGR_RESET ANSI_SGR);

	va_list ap;
	va_start(ap, format);

	vfprintf(trace->stream, format, ap);

	va_end(ap);

	fprintf(trace->stream, "\n");
}
