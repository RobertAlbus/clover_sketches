file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

hello_imgui_add_app(000_template ${SOURCE_CPP})
target_include_directories(000_template PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(000_template PRIVATE clover sketch_gui_libs)
