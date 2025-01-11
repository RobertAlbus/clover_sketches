file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_executable(005_snare_resonator ${SOURCE_CPP})
target_include_directories(005_snare_resonator PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(005_snare_resonator PRIVATE clover)
