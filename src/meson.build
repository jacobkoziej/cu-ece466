# SPDX-License-Identifier: GPL-3.0-or-later
#
# Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>

vcs_tag_cmd = git.found() ? [
        git,
        'describe',
        '--always',
        '--broken',
        '--dirty',
        '--tags'
] : ''

vcs_tag_h = vcs_tag(
        command : vcs_tag_cmd,
        input   : 'vcs-tag.h.in',
        output  : 'vcs-tag.h',
)


y_tab_c = custom_target(
        'y.tab.c',
        input  : 'parser.y',
        output : ['y.tab.c', 'y.tab.h'],
        command : [
                bison,
                '--header=@OUTPUT1@',
                '--output=@OUTPUT0@',
                '@INPUT0@',
        ],
)


lex_yy_c = custom_target(
        'lex.yy.c',
        input   : ['lexer.l', y_tab_c],
        output  : ['lex.yy.c', 'lex.yy.h'],
        command : [
                flex,
                '--outfile=@OUTPUT0@',
                '--header-file=@OUTPUT1@',
                '-Cef',
                '@INPUT0@',
        ],
)


jkcc_src = files(
        'ast.c',
        'jkcc.c',
        'lexer.c',
        'parser.c',
        'string.c',
)


executable(
        'jkcc',
        include_directories : jkcc_inc,
        sources : [
                'main.c',
                jkcc_src,
                vcs_tag_h,
                lex_yy_c,
                y_tab_c,
        ],
)
