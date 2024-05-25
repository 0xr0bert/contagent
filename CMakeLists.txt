cmake_minimum_required(VERSION 3.28)
project(contagent)
set(CMAKE_CXX_STANDARD 20)

# Dependencies

find_package(Boost 1.83 REQUIRED COMPONENTS iostreams)
find_package(PkgConfig REQUIRED)
pkg_check_modules(ZSTD REQUIRED IMPORTED_TARGET libzstd)
pkg_check_modules(GLOG REQUIRED IMPORTED_TARGET libglog)
pkg_check_modules(NLOHMANN_JSON REQUIRED IMPORTED_TARGET nlohmann_json)

# Contagent

add_library(contagent SHARED)
target_sources(contagent
    PUBLIC
    contagent-lib/src/agent.cc
    contagent-lib/src/behaviour.cc
    contagent-lib/src/belief.cc
    contagent-lib/src/configuration.cc
    contagent-lib/src/named.cc
    contagent-lib/src/runner.cc
    contagent-lib/src/summary.cc
    contagent-lib/src/uuidd.cc
    contagent-lib/src/json/agent_spec.cc
    contagent-lib/src/json/behaviour_spec.cc
    contagent-lib/src/json/belief_spec.cc
    contagent-lib/src/json/summary_spec.cc
    contagent-lib/src/json/zstd_boost.cc
    contagent-lib/src/json/zstd.cc
)

target_include_directories(contagent
    PUBLIC
    contagent-lib/include
)

target_link_libraries(contagent
    PUBLIC
    Boost::boost
    PkgConfig::ZSTD
    PkgConfig::GLOG
    PkgConfig::NLOHMANN_JSON
)

# Contagent-bin

add_executable(contagent-bin)

target_sources(contagent-bin
    PUBLIC
    contagent-bin/src/main.cc
)

target_include_directories(contagent-bin
    PUBLIC
    contagent-lib/include
    contagent-bin/include
    libs/clipp/include
)

target_link_libraries(contagent-bin
    PUBLIC
    contagent
    PkgConfig::GLOG
)
