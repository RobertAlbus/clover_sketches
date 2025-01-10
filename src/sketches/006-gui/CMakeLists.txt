file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

hello_imgui_add_app(006_gui ${SOURCE_CPP})
target_include_directories(006_gui PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(006_gui PRIVATE clover)
