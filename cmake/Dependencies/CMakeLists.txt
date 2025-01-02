option(CLOVER_SKETCHES_FETCH_DEPENDENCIES     "" ON)
option(CLOVER_SKETCHES_FETCH_CLOVER           "FetchContent for Clover"           ON)
option(CLOVER_SKETCHES_FETCH_GOOGLE_BENCHMARK "FetchContent for Google Benchmark" ${CLOVER_SKETCHES_BUILD_BENCHMARKS})
option(CLOVER_SKETCHES_FETCH_GOOGLE_TEST      "FetchContent for Google Test"      ${CLOVER_SKETCHES_BUILD_TESTS})

message("\n\n== Dependencies ==\n")

message("CLOVER_SKETCHES_FETCH_DEPENDENCIES:     ${CLOVER_SKETCHES_FETCH_DEPENDENCIES}")
message("CLOVER_SKETCHES_FETCH_CLOVER:           ${CLOVER_SKETCHES_FETCH_CLOVER}")
message("CLOVER_SKETCHES_FETCH_GOOGLE_BENCHMARK: ${CLOVER_SKETCHES_FETCH_GOOGLE_BENCHMARK}")
message("CLOVER_SKETCHES_FETCH_GOOGLE_TEST:      ${CLOVER_SKETCHES_FETCH_GOOGLE_TEST}")

include(Clover.cmake)
include(GoogleBenchmark.cmake)
include(GoogleTest.cmake)
message("\n--------\n")

