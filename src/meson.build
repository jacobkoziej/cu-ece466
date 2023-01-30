# SPDX-License-Identifier: GPL-3.0-or-later
#
# Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>

gen_lexer = custom_target(
        'gen-lexer',
        input   : 'lexer.lex',
        output  : ['lexer.c', 'yylex.h'],
        command : [
                flex,
                '--outfile=@OUTPUT0@',
                '--header-file=@OUTPUT1@',
                '--8bit',
                '--yylineno',
                '--noyywrap',
                '--reentrant',
                '-Cef',
                '@INPUT@',
        ],
)


jkcc_src = files(
)


executable(
        'jkcc',
        sources : [
                'jkcc.c',
                jkcc_src,
                gen_lexer,
        ],
)
