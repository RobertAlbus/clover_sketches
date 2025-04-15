file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

hello_imgui_add_app(020_composition_template ${SOURCE_CPP})
target_include_directories(020_composition_template PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(020_composition_template PRIVATE clover sketch_gui_libs)
