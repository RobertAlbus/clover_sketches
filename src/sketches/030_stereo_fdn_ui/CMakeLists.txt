file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_executable(030_stereo_fdn_ui ${SOURCE_CPP})
target_include_directories(030_stereo_fdn_ui PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(030_stereo_fdn_ui PRIVATE clover_sketches_lib clover imgui sketch_gui_libs concurrentqueue)
