file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_executable(028_transport ${SOURCE_CPP})
target_include_directories(028_transport PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(028_transport PRIVATE clover_sketches_lib clover imgui sketch_gui_libs concurrentqueue)
