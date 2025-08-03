file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

set(target_name 035_procedural)

add_executable(${target_name} ${SOURCE_CPP})
target_include_directories(${target_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(${target_name} PRIVATE clover_sketches_lib clover imgui sketch_gui_libs concurrentqueue)
target_compile_definitions(${target_name} PRIVATE PROJECT_NAME_STR="${target_name}")
