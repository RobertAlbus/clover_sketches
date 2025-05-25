file(GLOB_RECURSE CLOVER_SKETCHES_SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_library(clover_sketches_lib STATIC ${CLOVER_SKETCHES_SOURCE_CPP})
target_include_directories(clover_sketches_lib PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(clover_sketches_lib PRIVATE clover imgui sketch_gui_libs concurrentqueue Eigen3::Eigen)
