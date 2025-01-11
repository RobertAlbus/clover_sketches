file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

hello_imgui_add_app(007_snare_resonator ${SOURCE_CPP})
target_include_directories(007_snare_resonator PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(007_snare_resonator PRIVATE clover)
