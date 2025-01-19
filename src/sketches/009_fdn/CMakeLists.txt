file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

hello_imgui_add_app(009_fdn ${SOURCE_CPP})
target_include_directories(009_fdn PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(009_fdn PRIVATE clover)
