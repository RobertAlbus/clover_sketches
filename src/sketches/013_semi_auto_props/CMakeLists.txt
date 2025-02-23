file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_executable(013_semi_auto_props ${SOURCE_CPP})
target_include_directories(013_semi_auto_props PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(013_semi_auto_props PRIVATE clover)
