# SPDX-License-Identifier: GPL-3.0-or-later
#
# Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>

tests = {
        'lexer' : {
                'args' : [
                        files(
                                'lexer.d/character_constants',
                                'lexer.d/floating_constants',
                                'lexer.d/identifiers',
                                'lexer.d/integer_constants',
                                'lexer.d/keywords',
                                'lexer.d/punctuators',
                                'lexer.d/string_literals',
                        ),
                ],
        },
}


if not get_option('tests').disabled() and cmocka_dep.found()
        foreach name, args : tests
                exe = executable(
                        name.underscorify(),
                        dependencies        : cmocka_dep,
                        include_directories : jkcc_inc,
                        sources      : [
                                name + '.c',
                                jkcc_src,
                                lex_yy_c,
                                y_tab_c,
                        ]
                )

                test(
                        name,
                        exe,
                        args : args.get('args', [ ]),
                )
        endforeach
endif
