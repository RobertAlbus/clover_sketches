file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

hello_imgui_add_app(018_env_bp ${SOURCE_CPP})
target_include_directories(018_env_bp PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(018_env_bp PRIVATE clover sketch_gui_libs atomic)

# target_compile_options(018_env_bp PRIVATE -fsanitize=undefined -fno-omit-frame-pointer)
