option(CLOVER_SKETCHES_FETCH_DEPENDENCIES     "" ON)

message("\n\n== Dependencies ==\n")

message("CLOVER_SKETCHES_FETCH_DEPENDENCIES:     ${CLOVER_SKETCHES_FETCH_DEPENDENCIES}")

file(GLOB DEPENDENCIES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cmake
)

foreach(CMAKE_FILE IN LISTS DEPENDENCIES)
    include(${CMAKE_FILE})
endforeach()

message("\n--------\n")

