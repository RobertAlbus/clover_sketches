file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_executable(027_gpeq ${SOURCE_CPP})
target_include_directories(027_gpeq PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(027_gpeq PRIVATE clover_sketches_lib clover imgui sketch_gui_libs concurrentqueue)
