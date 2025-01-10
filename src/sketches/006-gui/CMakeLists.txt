set(GLFW_BUILD_TESTS OFF CACHE BOOL "Disable GLFW tests" FORCE)
FetchContent_Declare(
    glfw3
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.3.4
)
FetchContent_MakeAvailable(glfw3)
message(STATUS ${glfw3_BINARY_DIR})
set(glfw3_DIR ${glfw3_BINARY_DIR}/src)

set(HELLOIMGUI_USE_GLFW3 ON  CACHE BOOL "HELLOIMGUI_USE_GLFW3" FORCE)
set(HELLOIMGUI_INSTALL   OFF CACHE BOOL "HELLOIMGUI_INSTALL" FORCE)

FetchContent_Declare(
    hello_imgui
    GIT_REPOSITORY https://github.com/pthom/hello_imgui.git
    GIT_TAG        13e6bd4f6e6b1e7dee7a0bb01b08ada49c7f4b30
)
FetchContent_MakeAvailable(hello_imgui)
message(STATUS ${hello_imgui_BINARY_DIR})


# --------
file(GLOB_RECURSE SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

hello_imgui_add_app(006_gui ${SOURCE_CPP})
target_include_directories(006_gui PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(006_gui PRIVATE clover)
