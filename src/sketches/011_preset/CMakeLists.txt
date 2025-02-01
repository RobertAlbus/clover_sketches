file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

hello_imgui_add_app(011_presets ${SOURCE_CPP})
target_include_directories(011_presets PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(011_presets PRIVATE clover)
