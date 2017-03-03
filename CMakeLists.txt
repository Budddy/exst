cmake_minimum_required(VERSION 3.2)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/tools")
project(exst)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11  -W -Wall -O3 -DNDEBUG")

option(tests "turn on to build tests" OFF)

if(tests)
    # flags to generate coverage
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS
            "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
endif(tests)

find_package(TBB)

if (NOT TBB_FOUND)
    MESSAGE(STATUS "TBB not found. Building without threads.")
    add_definitions(-DWITH_THREADS=0)
else (NOT TBB_FOUND)
    MESSAGE(STATUS "TBB found. Building with threads.")
    add_definitions(-DWITH_THREADS=1)
    link_directories(${TBB_LIBRARY_DIRS})
endif (NOT TBB_FOUND)

include_directories(libclasp libprogram_opts lib_exst)
if(tests)
    include_directories(lib_exst/tests)

    # build exst tests
    add_subdirectory(lib_exst/tests)
    add_subdirectory(libclasp)
    add_subdirectory(libprogram_opts)
    add_subdirectory(lib_exst)
else(tests)
    add_subdirectory(libclasp)
    add_subdirectory(libprogram_opts)
    add_subdirectory(lib_exst)
    add_subdirectory(app)
endif(tests)


