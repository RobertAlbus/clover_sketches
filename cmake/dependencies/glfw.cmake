if(CLOVER_SKETCHES_FETCH_DEPENDENCIES)
    include(FetchContent)


    set(CLOVER_SKETCHES_FETCH_GLFW3_TAG "3.3.4")
    set(CLOVER_SKETCHES_FETCH_HELLO_IMGUI_TAG "13e6bd4f6e6b1e7dee7a0bb01b08ada49c7f4b30")

    set(GLFW_BUILD_TESTS OFF CACHE BOOL "Disable GLFW tests" FORCE)
    set(HELLOIMGUI_HAS_OPENGL3 ON  CACHE BOOL "HELLOIMGUI_HAS_OPENGL3" FORCE)
    set(HELLOIMGUI_USE_GLFW3 ON  CACHE BOOL "HELLOIMGUI_USE_GLFW3" FORCE)
    set(HELLOIMGUI_INSTALL   OFF CACHE BOOL "HELLOIMGUI_INSTALL" FORCE)

    FetchContent_Declare(
        glfw3
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        ${CLOVER_SKETCHES_FETCH_GLFW3_TAG}
    )
    FetchContent_Declare(
        hello_imgui
        GIT_REPOSITORY https://github.com/pthom/hello_imgui.git
        GIT_TAG        ${CLOVER_SKETCHES_FETCH_HELLO_IMGUI_TAG}
    )

    set(OLD_CMAKE_MESSAGE_LOG_LEVEL ${CMAKE_MESSAGE_LOG_LEVEL})
    set(CMAKE_MESSAGE_LOG_LEVEL WARNING)

    FetchContent_MakeAvailable(glfw3)
    set(glfw3_DIR ${glfw3_BINARY_DIR}/src)
    FetchContent_MakeAvailable(hello_imgui)

    set_target_properties(glfw PROPERTIES CXX_CLANG_TIDY "")
    set_target_properties(imgui PROPERTIES CXX_CLANG_TIDY "")
    set_target_properties(hello_imgui PROPERTIES CXX_CLANG_TIDY "")
    set_target_properties(stb_hello_imgui PROPERTIES CXX_CLANG_TIDY "")
        
    set(CMAKE_MESSAGE_LOG_LEVEL ${OLD_CMAKE_MESSAGE_LOG_LEVEL})

endif()
