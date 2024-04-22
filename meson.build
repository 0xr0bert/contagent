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
check = dependency('check')
subdir('src')
subdir('test')
libcontagent = library(
    'contagent', sources, include_directories : incdir, dependencies: [m_dep, glib])
executable(
    'contagent_test',
    test_srcs,
    include_directories : incdir,
    dependencies: [m_dep, glib, check],
    link_with : [libcontagent]
)