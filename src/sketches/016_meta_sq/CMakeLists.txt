file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

hello_imgui_add_app(016_meta_sq ${SOURCE_CPP})
target_include_directories(016_meta_sq PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(016_meta_sq PRIVATE clover)

# target_compile_options(016_meta_sq PRIVATE -fsanitize=undefined -fno-omit-frame-pointer)


