# SPDX-License-Identifier: GPL-3.0-or-later
#
# Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>

lex_yy_c = custom_target(
        'lex.yy.c',
        input   : 'lexer.l',
        output  : ['lex.yy.c', 'lex.yy.h'],
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
                lex_yy_c,
        ],
)
