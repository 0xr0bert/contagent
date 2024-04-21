project(
    'contagent',
    'c',
    default_options: [
        'c_std=c99',
        'warning_level=3'
    ]
)

incdir = include_directories('include')
glib = dependency('glib-2.0')
library('contagent', 'contagent.c', include_directories : incdir, dependencies: glib)