file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_executable(00_template ${SOURCE_CPP})
target_include_directories(00_template PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(00_template PRIVATE clover)
