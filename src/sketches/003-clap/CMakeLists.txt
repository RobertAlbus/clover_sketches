file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_executable(003_clap ${SOURCE_CPP})
target_include_directories(003_clap PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(003_clap PRIVATE clover)
