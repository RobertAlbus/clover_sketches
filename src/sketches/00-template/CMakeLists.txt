file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_executable(sketch_00 ${SOURCE_CPP})
target_include_directories(sketch_00 PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(sketch_00 PRIVATE clover)
add_dependencies(sketch_00 clover)

