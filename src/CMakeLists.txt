add_subdirectory(clover_sketches_lib)

if(CLOVER_SKETCHES_BUILD_BENCHMARKS)
    add_subdirectory(clover_sketches_benchmark)
endif()
if(CLOVER_SKETCHES_BUILD_TESTS)
    add_subdirectory(clover_sketches_test)
endif()
