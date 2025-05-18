file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_executable(024_kick_study ${SOURCE_CPP})
target_include_directories(024_kick_study PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(024_kick_study PRIVATE clover_sketches_lib clover imgui sketch_gui_libs concurrentqueue)
