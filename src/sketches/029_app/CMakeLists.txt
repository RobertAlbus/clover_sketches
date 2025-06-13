file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_executable(029_app ${SOURCE_CPP})
target_include_directories(029_app PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(029_app PRIVATE clover_sketches_lib clover imgui sketch_gui_libs concurrentqueue)
