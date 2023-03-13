# SPDX-License-Identifier: GPL-3.0-or-later
#
# Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>

project(
        'cu-ece466',
        'c',
        version : 'v0.0.0',
        license : 'GPL-3.0-or-later',
        default_options : [
                'c_std=c11',
                'warning_level=3',
        ],
        meson_version : '>=1.0.0',
)


# dependencies
cmocka_dep = dependency(
        'cmocka',
        required : get_option('tests'),
)


# programs
bison    = find_program('bison')
flex     = find_program('flex')
git      = find_program('git', required : false)
realpath = find_program('realpath', required : false)


# global arguments
add_global_arguments('-D_DEFAULT_SOURCE', language : 'c')

if (realpath.found())
        relative_source_root = run_command(
                realpath,
                '--relative-to=' + meson.project_build_root(),
                meson.project_source_root(),
                capture : true,
                check   : true,
        )

        relative_source_root = relative_source_root.stdout().strip() + '/'

        add_global_arguments(
                '-ffile-prefix-map=' + relative_source_root + '=',
                language : 'c'
        )
endif


subdir('include')
subdir('src')
subdir('tests')
