// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * constant.h -- constant types
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_CONSTANT_H
#define JKCC_CONSTANT_H


#include <uchar.h>
#include <wchar.h>

#include <jkcc/string.h>


enum integer_constant_e {
	INT,
	UNSIGNED_INT,
	LONG_INT,
	UNSIGNED_LONG_INT,
	LONG_LONG_INT,
	UNSIGNED_LONG_LONG_INT,
};

enum floating_constant_e {
	FLOAT,
	DOUBLE,
	LONG_DOUBLE,
};

enum character_constant_e {
	UNSIGNED_CHAR,
	WCHAR_T,
	CHAR16_T,
	CHAR32_T,
};

enum string_literal_e {
	STRING_CHAR,
	STRING_UTF_8,
	STRING_CHAR16_T,
	STRING_CHAR32_T,
	STRING_WCHAR_T,
};


typedef struct identifier_s {
	string_t IDENTIFIER;
	string_t text;
} identifier_t;

typedef struct integer_constant_s {
	enum integer_constant_e type;
	union {
		int                    INT;
		unsigned int           UNSIGNED_INT;
		long int               LONG_INT;
		unsigned long int      UNSIGNED_LONG_INT;
		long long int          LONG_LONG_INT;
		unsigned long long int UNSIGNED_LONG_LONG_INT;
	};
	string_t text;
} integer_constant_t;

typedef struct floating_constant_s {
	enum floating_constant_e type;
	union {
		float       FLOAT;
		double      DOUBLE;
		long double LONG_DOUBLE;
	};
	string_t text;
} floating_constant_t;

typedef struct character_constant_s {
	enum character_constant_e type;
	union {
		unsigned char UNSIGNED_CHAR;
		wchar_t       WCHAR_T;
		char16_t      CHAR16_T;
		char32_t      CHAR32_T;
	};
	string_t text;
} character_constant_t;

typedef struct string_literal_s {
	enum string_literal_e encoding;
	string_t              string;
	string_t              text;
} string_literal_t;


#endif  /* JKCC_CONSTANT_H */
