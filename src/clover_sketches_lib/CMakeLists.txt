file(GLOB_RECURSE CLOVER_SKETCHES_SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_library(clover_sketches STATIC ${CLOVER_SKETCHES_SOURCE_CPP})
target_include_directories(clover_sketches PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(clover_sketches PRIVATE clover)

