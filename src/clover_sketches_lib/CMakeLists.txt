file(GLOB_RECURSE CLOVER_SKETCHES_SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_library(libclover_sketches STATIC ${CLOVER_SKETCHES_SOURCE_CPP})
target_include_directories(libclover_sketches PUBLIC ${CMAKE_CURRENT_LIST_DIR})
