if(CLOVER_SKETCHES_FETCH_DEPENDENCIES AND CLOVER_SKETCHES_FETCH_GOOGLE_TEST)
    set(CLOVER_SKETCHES_FETCH_GOOGLE_TEST_VERSION_TAG "v1.15.2")

    set(BUILD_GMOCK   OFF)
    set(INSTALL_GTEST OFF)

    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_SHALLOW TRUE
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG ${CLOVER_SKETCHES_FETCH_GOOGLE_TEST_VERSION_TAG}
    )

    set(old_CMAKE_MESSAGE_LOG_LEVEL ${CMAKE_MESSAGE_LOG_LEVEL})
    set(CMAKE_MESSAGE_LOG_LEVEL WARNING)

    FetchContent_MakeAvailable(googletest)

    # Disable clang-tidy for this dependency
    set_target_properties(gtest         PROPERTIES CXX_CLANG_TIDY "")
    set_target_properties(gtest_main    PROPERTIES CXX_CLANG_TIDY "")

    set(CMAKE_MESSAGE_LOG_LEVEL ${old_CMAKE_MESSAGE_LOG_LEVEL})

endif()
