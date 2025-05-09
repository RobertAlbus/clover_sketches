file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_executable(023_fdn ${SOURCE_CPP})
target_include_directories(023_fdn PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(023_fdn PRIVATE clover_sketches_lib clover imgui sketch_gui_libs concurrentqueue)
