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

version_h = vcs_tag(
        command : vcs_tag_cmd,
        input   : 'version.h.in',
        output  : 'version.h',
)


jkcc_config_h = configure_file(
        input : 'config.h.in',
        output : 'config.h',
        configuration : {
                'JKCC_CONFIG_AST_PRINT_LOCATION' : get_option('ast-print-location'),
                'JKCC_CONFIG_OPTION_TRACE'       : get_option('trace'),
        },
)
