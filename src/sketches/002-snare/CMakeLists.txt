file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_executable(002_snare ${SOURCE_CPP})
target_include_directories(002_snare PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(002_snare PRIVATE clover)
