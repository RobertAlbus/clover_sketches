file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

hello_imgui_add_app(004_cymbal ${SOURCE_CPP})
target_include_directories(004_cymbal PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(004_cymbal PRIVATE clover)
