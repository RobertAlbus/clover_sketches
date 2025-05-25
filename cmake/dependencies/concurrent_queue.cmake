
if(CLOVER_SKETCHES_FETCH_DEPENDENCIES)
    set(CLOVER_SKETCHES_FETCH_CONCURRENT_QUEUE_VERSION_TAG "v1.0.4")
    
    include(FetchContent)

    FetchContent_Declare(
        concurrentqueue
        GIT_SHALLOW TRUE
        GIT_REPOSITORY https://github.com/cameron314/concurrentqueue.git
        GIT_TAG ${CLOVER_SKETCHES_FETCH_CONCURRENT_QUEUE_VERSION_TAG}
    )

    set(old_CMAKE_MESSAGE_LOG_LEVEL ${CMAKE_MESSAGE_LOG_LEVEL})
    set(CMAKE_MESSAGE_LOG_LEVEL WARNING)

    FetchContent_MakeAvailable(concurrentqueue)

    # Disable clang-tidy for this dependency
    set_target_properties(benchmark      PROPERTIES CXX_CLANG_TIDY "")
    set_target_properties(benchmark_main PROPERTIES CXX_CLANG_TIDY "")

    set(CMAKE_MESSAGE_LOG_LEVEL ${old_CMAKE_MESSAGE_LOG_LEVEL})

endif()
