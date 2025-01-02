file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_executable(03_clap ${SOURCE_CPP})
target_include_directories(03_clap PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(03_clap PRIVATE clover)
