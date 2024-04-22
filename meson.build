project(
    'contagent',
    'c',
    default_options: [
        'c_std=c99',
        'warning_level=3'
    ]
)

incdir = include_directories('include')
cc = meson.get_compiler('c')
m_dep = cc.find_library('m', required : true)
glib = dependency('glib-2.0')
subdir('src')
library('contagent', sources, include_directories : incdir, dependencies: [m_dep, glib])