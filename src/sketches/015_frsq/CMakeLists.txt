file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

hello_imgui_add_app(015_frsq ${SOURCE_CPP})
target_include_directories(015_frsq PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(015_frsq PRIVATE clover)

# target_compile_options(015_frsq PRIVATE -fsanitize=undefined -fno-omit-frame-pointer)


