file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_executable(021_sd_template ${SOURCE_CPP})
target_include_directories(021_sd_template PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(021_sd_template PRIVATE clover imgui sketch_gui_libs)
