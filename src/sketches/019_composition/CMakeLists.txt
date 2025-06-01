file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

hello_imgui_add_app(019_composition ${SOURCE_CPP})
target_include_directories(019_composition PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(019_composition PRIVATE clover clover_sketches_lib hello_imgui sketch_gui_libs)
