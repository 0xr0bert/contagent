project('contagent', 'cpp',
        version : '1.0.0',
        default_options : ['warning_level=3', 'cpp_std=c++20'])

subdir('contagent-lib')

boost_dep = dependency(
        'boost',
        modules : ['iostreams']
)
zstd_dep = dependency('libzstd')
glog_dep = dependency('libglog')
contagent_lib =library(
        'contagent',
        lib_sources,
        include_directories : incdir,
        dependencies : [boost_dep, glog_dep, zstd_dep]
)

clipp_dir = include_directories('libs/clipp/include')

subdir('contagent-bin')
contagent_bin = executable(
        'contagentsim',
        bin_sources,
        install : true,
        include_directories : [incdir, clipp_dir],
        link_with : contagent_lib,
        dependencies : glog_dep
)

test('test', contagent_bin)