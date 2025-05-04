find_program(CLOVER_SKETCHES_CTEST_EXECUTABLE ctest)
if(NOT CLOVER_SKETCHES_CTEST_EXECUTABLE)
    message(FATAL_ERROR "ctest not found!")
endif()

# set(CMAKE_CTEST_ARGUMENTS "")

file(GLOB_RECURSE CLOVER_SKETCHES_SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.test.cpp
    ${CMAKE_CURRENT_LIST_DIR}/**/*.test.cpp
)

add_executable(clover_sketches_test ${CLOVER_SKETCHES_SOURCE_CPP})
target_link_libraries(clover_sketches_test PRIVATE GTest::gtest GTest::gtest_main clover_sketches_lib)

include(GoogleTest)
gtest_discover_tests(clover_sketches_test)
