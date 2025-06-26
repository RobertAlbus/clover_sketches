file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_executable(032_meter ${SOURCE_CPP})
target_include_directories(032_meter PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(032_meter PRIVATE clover_sketches_lib clover imgui sketch_gui_libs concurrentqueue)
