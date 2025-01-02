file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_executable(01_kick ${SOURCE_CPP})
target_include_directories(01_kick PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(01_kick PRIVATE clover)
