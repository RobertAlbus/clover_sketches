file(GLOB_RECURSE CLOVER_SKETCHES_BENCHMARK_SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_executable(clover_sketches_benchmark ${CLOVER_SKETCHES_BENCHMARK_SOURCE_CPP})
target_include_directories(clover_sketches_benchmark PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(clover_sketches_benchmark PUBLIC benchmark::benchmark libclover_sketches)