file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

hello_imgui_add_app(017_peqn ${SOURCE_CPP})
target_include_directories(017_peqn PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(017_peqn PRIVATE clover sketch_gui_libs atomic)

# target_compile_options(017_peqn PRIVATE -fsanitize=undefined -fno-omit-frame-pointer)


