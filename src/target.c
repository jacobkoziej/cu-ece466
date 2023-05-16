// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * target.c -- garbage target code generation
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/target.h>

#include <stdio.h>

#include <jkcc/ir.h>
#include <jkcc/version.h>


int target_gen(FILE *stream, target_t target, ir_unit_t *ir_unit)
{
	(void) target;
	(void) ir_unit;

	ast_t **extern_declaration = ir_unit->extern_declaration.buf;
	for (size_t i = 0; i < ir_unit->extern_declaration.use; i++) {
		if (target_extern_declaration[target](
			stream,
			extern_declaration[i])) return -1;

		fprintf(stream, "\n");
	}

	fprintf(stream, "\t.ident\t\"jkcc version %s\"\n", JKCC_VERSION);
	fprintf(stream, "\t.section\t\".note.GNU-stack\",\"\",@progbits\n");

	return 0;
}
