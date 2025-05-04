file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

hello_imgui_add_app(014_nx_osc ${SOURCE_CPP})
target_include_directories(014_nx_osc PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(014_nx_osc PRIVATE clover clover_sketches_lib)

# target_compile_options(014_nx_osc PRIVATE -fsanitize=undefined -fno-omit-frame-pointer)


