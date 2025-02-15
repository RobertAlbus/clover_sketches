file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

hello_imgui_add_app(012_preset_fdn ${SOURCE_CPP})
target_include_directories(012_preset_fdn PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(012_preset_fdn PRIVATE clover)

# target_compile_options(012_preset_fdn PRIVATE -fsanitize=undefined -fno-omit-frame-pointer)


