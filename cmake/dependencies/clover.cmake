
if(CLOVER_SKETCHES_FETCH_DEPENDENCIES)
    set(CLOVER_SKETCHES_FETCH_CLOVER_VERSION_TAG "v3.1")

    set(CLOVER_BUILD_TESTS      OFF)
    set(CLOVER_BUILD_BENCHMARKS OFF)
    set(CLOVER_BUILD_DEMO       OFF)
    set(CLOVER_ENABLE_CLANG_TIDY OFF)

    include(FetchContent)
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
