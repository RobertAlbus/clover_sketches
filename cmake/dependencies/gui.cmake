if(CLOVER_SKETCHES_FETCH_DEPENDENCIES)
    include(FetchContent)

    # --------------------------------
    # fetch

    set(BUILD_SHARED_LIBS OFF)
    set(GLFW_BUILD_EXAMPLES OFF)
    set(GLFW_BUILD_TESTS OFF)
    set(GLFW_BUILD_DOCS OFF)
    set(GLFW_INSTALL OFF)
    FetchContent_Declare(
        glfw3
        GIT_SHALLOW TRUE
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        "3.3.4"
    )

    # --------------------------------
    # make available

    set(OLD_CMAKE_MESSAGE_LOG_LEVEL ${CMAKE_MESSAGE_LOG_LEVEL})
    set(CMAKE_MESSAGE_LOG_LEVEL WARNING)
    
    FetchContent_MakeAvailable(glfw3)
    set(glfw3_DIR ${glfw3_BINARY_DIR}/src)
    set_target_properties(glfw PROPERTIES CXX_CLANG_TIDY "")
    
    set(CMAKE_MESSAGE_LOG_LEVEL ${OLD_CMAKE_MESSAGE_LOG_LEVEL})



    # --------------------------------
    # additional libraries

    FetchContent_Declare(
        imgui_knobs
        GIT_SHALLOW TRUE
        GIT_REPOSITORY https://github.com/altschuler/imgui-knobs.git
        GIT_TAG        "a0768e19300268d15d748b5f67bc30deda75e11a"
    )
    FetchContent_MakeAvailable(imgui_knobs)

    
    FetchContent_Declare(
        implot
        GIT_SHALLOW TRUE
        GIT_REPOSITORY https://github.com/epezent/implot.git
        GIT_TAG        "3da8bd34299965d3b0ab124df743fe3e076fa222"
    )
    FetchContent_MakeAvailable(implot)

    
    FetchContent_Declare(
        implot3d
        GIT_SHALLOW TRUE
        GIT_REPOSITORY https://github.com/brenocq/implot3d.git
        GIT_TAG        "743cf425d74568986e4a16dd0ca9e021f0cdfb34"
    )
    FetchContent_MakeAvailable(implot3d)

    
    file(GLOB IMGUI_KNOBS_SOURCES ${imgui_knobs_SOURCE_DIR}/*.cpp)
    add_library(imgui_knobs_lib STATIC ${IMGUI_KNOBS_SOURCES})
    target_include_directories( imgui_knobs_lib PUBLIC ${imgui_knobs_SOURCE_DIR})
    target_link_libraries(      imgui_knobs_lib PRIVATE imgui                   )
    set_target_properties(      imgui_knobs_lib PROPERTIES CXX_CLANG_TIDY ""    )
    
    file(GLOB IMPLOT_SOURCES ${implot_SOURCE_DIR}/*.cpp)
    add_library(implot_lib STATIC ${IMPLOT_SOURCES})
    target_include_directories( implot_lib PUBLIC ${implot_SOURCE_DIR} )
    target_link_libraries(      implot_lib PRIVATE imgui               )
    set_target_properties(      implot_lib PROPERTIES CXX_CLANG_TIDY "")
    
    file(GLOB IMPLOT3D_SOURCES ${implot3d_SOURCE_DIR}/*.cpp)
    add_library(implot3d_lib STATIC ${IMPLOT3D_SOURCES})
    target_include_directories( implot3d_lib PUBLIC ${implot3d_SOURCE_DIR})
    target_link_libraries(      implot3d_lib PRIVATE imgui                )
    set_target_properties(      implot3d_lib PROPERTIES CXX_CLANG_TIDY "" )
    
    add_library(sketch_gui_libs INTERFACE)
    target_link_libraries(sketch_gui_libs INTERFACE
        imgui_knobs_lib
        implot_lib
        implot3d_lib
    )
    
endif()
