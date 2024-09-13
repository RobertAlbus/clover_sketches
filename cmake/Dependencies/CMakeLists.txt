option(CLOVER_SKETCHES_FETCH_DEPENDENCIES     "" ON)
option(CLOVER_SKETCHES_FETCH_GOOGLE_BENCHMARK "FetchContent for Google Benchmark" ON)
option(CLOVER_SKETCHES_FETCH_GOOGLE_TEST      "FetchContent for Google Test"      ON)

message("\n\n== Dependencies ==\n")

message("CLOVER_SKETCHES_FETCH_DEPENDENCIES:     ${CLOVER_SKETCHES_FETCH_DEPENDENCIES}")
message("CLOVER_SKETCHES_FETCH_GOOGLE_BENCHMARK: ${CLOVER_SKETCHES_FETCH_GOOGLE_BENCHMARK}")
message("CLOVER_SKETCHES_FETCH_GOOGLE_TEST:      ${CLOVER_SKETCHES_FETCH_GOOGLE_TEST}")

include(GoogleBenchmark.cmake)
include(GoogleTest.cmake)
message("\n--------\n")

