file(GLOB LIBCLASP_TEST_SRC *.cpp)
include_directories(${CMAKE_SOURCE_DIR}/libclasp)
add_executable(libclasp_test ${LIBCLASP_TEST_SRC})
target_link_libraries(libclasp_test libclasp lib_exst cppunit htd tbb)

set_target_properties(libclasp_test PROPERTIES OUTPUT_NAME libclasp_test)
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
