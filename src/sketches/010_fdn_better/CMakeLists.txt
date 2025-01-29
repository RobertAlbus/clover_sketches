file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

hello_imgui_add_app(010_fdn_better ${SOURCE_CPP})
target_include_directories(010_fdn_better PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(010_fdn_better PRIVATE clover)
