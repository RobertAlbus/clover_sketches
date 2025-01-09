file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_executable(06_gui ${SOURCE_CPP})
target_include_directories(06_gui PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(06_gui PRIVATE clover)
