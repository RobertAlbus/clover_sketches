file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_executable(031_reso_snare ${SOURCE_CPP})
target_include_directories(031_reso_snare PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(031_reso_snare PRIVATE clover_sketches_lib clover imgui sketch_gui_libs concurrentqueue)
