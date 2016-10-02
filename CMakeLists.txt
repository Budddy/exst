cmake_minimum_required(VERSION 3.2)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/tools")
project(exst)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -std=c++11 -O3 -DNDEBUG")
find_package(TBB)
include_directories(libclasp libprogram_opts lib_exst)
option(tests "turn on to build tests" OFF)

if (NOT TBB_FOUND)
    MESSAGE(STATUS "TBB not found. Building without threads.")
    add_definitions(-DWITH_THREADS=0)
else (NOT TBB_FOUND)
    MESSAGE(STATUS "TBB found. Building with threads.")
    add_definitions(-DWITH_THREADS=1)
    link_directories(${TBB_LIBRARY_DIRS})
endif (NOT TBB_FOUND)

add_subdirectory(libclasp)
add_subdirectory(libprogram_opts)
add_subdirectory(lib_exst)
add_subdirectory(app)

if(tests)

    # flags to generate coverage
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS
            "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
    include_directories(lib_exst/tests libclasp/tests)

    # build exst tests
    add_subdirectory(lib_exst/tests)
    # build clasp tests
    add_subdirectory(libclasp/tests)
endif(tests)

