
if(CLOVER_SKETCHES_FETCH_DEPENDENCIES)
    set(CLOVER_SKETCHES_FETCH_GOOGLE_BENCHMARK_VERSION_TAG "v1.9.0")
    
    set(BENCHMARK_ENABLE_TESTING    OFF)
    set(BENCHMARK_ENABLE_INSTALL    OFF)
    set(BENCHMARK_ENABLE_EXCEPTIONS OFF)
    set(BENCHMARK_INSTALL_DOCS      OFF)

    include(FetchContent)
    FetchContent_Declare(
        benchmark
        GIT_SHALLOW TRUE
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG ${CLOVER_SKETCHES_FETCH_GOOGLE_BENCHMARK_VERSION_TAG}
    )

    set(old_CMAKE_MESSAGE_LOG_LEVEL ${CMAKE_MESSAGE_LOG_LEVEL})
    set(CMAKE_MESSAGE_LOG_LEVEL WARNING)

    FetchContent_MakeAvailable(benchmark)

    # Disable clang-tidy for this dependency
    set_target_properties(benchmark      PROPERTIES CXX_CLANG_TIDY "")
    set_target_properties(benchmark_main PROPERTIES CXX_CLANG_TIDY "")

    set(CMAKE_MESSAGE_LOG_LEVEL ${old_CMAKE_MESSAGE_LOG_LEVEL})

endif()
