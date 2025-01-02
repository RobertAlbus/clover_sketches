set(CLOVER_SKETCHES_FETCH_CLOVER_VERSION "fetch_content")
set(CLOVER_SKETCHES_FETCH_CLOVER_VERSION_TAG "${CLOVER_SKETCHES_FETCH_CLOVER_VERSION}")

if(CLOVER_SKETCHES_FETCH_DEPENDENCIES AND CLOVER_SKETCHES_FETCH_CLOVER)

    set(CLOVER_BUILD_TESTS      OFF)
    set(CLOVER_BUILD_BENCHMARKS OFF)
    set(CLOVER_BUILD_DEMO       OFF)

    include(FetchContent)

    message("\n-- Clover (${CLOVER_SKETCHES_FETCH_CLOVER_VERSION} via FetchContent)")

    FetchContent_Declare(
        Clover
        GIT_REPOSITORY https://github.com/RobertAlbus/clover.git
        GIT_TAG ${CLOVER_SKETCHES_FETCH_CLOVER_VERSION_TAG}
    )
    
    set(old_CMAKE_MESSAGE_LOG_LEVEL ${CMAKE_MESSAGE_LOG_LEVEL})
    set(CMAKE_MESSAGE_LOG_LEVEL WARNING)

    FetchContent_MakeAvailable(Clover)
    
    # Disable clang-tidy for this dependency
    set_target_properties(clover PROPERTIES CXX_CLANG_TIDY "")
    
    set(CMAKE_MESSAGE_LOG_LEVEL ${old_CMAKE_MESSAGE_LOG_LEVEL})
endif()
