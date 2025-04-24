file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_executable(022_sd_reso_chords ${SOURCE_CPP})
target_include_directories(022_sd_reso_chords PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(022_sd_reso_chords PRIVATE clover imgui sketch_gui_libs concurrentqueue)
