file(GLOB_RECURSE SKETCH_CMAKES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/**/CMakeLists.txt
)

get_property(all_targets DIRECTORY PROPERTY BUILDSYSTEM_TARGETS)

message(STATUS "List of all targets:")
foreach(target IN LISTS all_targets)
    message(STATUS "  ${target}")
endforeach()

foreach(CMAKELIST IN LISTS SKETCH_CMAKES)
    get_filename_component(DIR ${CMAKELIST} DIRECTORY)
    add_subdirectory(${DIR})
endforeach()

