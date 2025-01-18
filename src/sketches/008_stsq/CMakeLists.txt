file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

hello_imgui_add_app(008_stsq ${SOURCE_CPP})
target_include_directories(008_stsq PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(008_stsq PRIVATE clover)
