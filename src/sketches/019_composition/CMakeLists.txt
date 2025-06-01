file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_executable(019_composition ${SOURCE_CPP})
target_include_directories(019_composition PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(019_composition PRIVATE clover clover_sketches_lib imgui sketch_gui_libs)
