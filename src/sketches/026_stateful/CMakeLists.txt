file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_executable(026_stateful ${SOURCE_CPP})
target_include_directories(026_stateful PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(026_stateful PRIVATE clover_sketches_lib clover imgui sketch_gui_libs concurrentqueue)
