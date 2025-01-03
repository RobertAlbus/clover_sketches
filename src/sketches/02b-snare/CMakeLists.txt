file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_executable(02b_snare ${SOURCE_CPP})
target_include_directories(02b_snare PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(02b_snare PRIVATE clover)
