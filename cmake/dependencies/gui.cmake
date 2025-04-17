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
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        "3.3.4"
    )
    set(HELLOIMGUI_IMGUI_SOURCE_DIR ${imgui_SOURCE_DIR} CACHE PATH "Path to Dear ImGui")
    set(HELLOIMGUI_BUILD_IMGUI      OFF CACHE BOOL "HELLOIMGUI_BUILD_IMGUI" FORCE)
    set(HELLOIMGUI_HAS_OPENGL3      ON  CACHE BOOL "HELLOIMGUI_HAS_OPENGL3" FORCE)
    set(HELLOIMGUI_USE_GLFW3        ON  CACHE BOOL "HELLOIMGUI_USE_GLFW3" FORCE)
    set(HELLOIMGUI_INSTALL          OFF CACHE BOOL "HELLOIMGUI_INSTALL" FORCE)
    FetchContent_Declare(
        hello_imgui
        GIT_REPOSITORY https://github.com/pthom/hello_imgui.git
        GIT_TAG        "1d8abf8fa1a3daf001a41f1fe5ceb2cc58f7980f"
    )

    # --------------------------------
    # make available

    set(OLD_CMAKE_MESSAGE_LOG_LEVEL ${CMAKE_MESSAGE_LOG_LEVEL})
    set(CMAKE_MESSAGE_LOG_LEVEL WARNING)

    FetchContent_MakeAvailable(glfw3)
    set(glfw3_DIR ${glfw3_BINARY_DIR}/src)
    set_target_properties(glfw PROPERTIES CXX_CLANG_TIDY "")

    FetchContent_MakeAvailable(hello_imgui)


    set_target_properties(imgui PROPERTIES CXX_CLANG_TIDY "")
    set_target_properties(hello_imgui PROPERTIES CXX_CLANG_TIDY "")
    set_target_properties(stb_hello_imgui PROPERTIES CXX_CLANG_TIDY "")
        
    set(CMAKE_MESSAGE_LOG_LEVEL ${OLD_CMAKE_MESSAGE_LOG_LEVEL})



    # --------------------------------
    # additional libraries

    FetchContent_Declare(
        imgui_knobs
        GIT_REPOSITORY https://github.com/altschuler/imgui-knobs.git
        GIT_TAG        "091dda7aed58fd44535023c61595facd32aea7fc"
    )
    FetchContent_MakeAvailable(imgui_knobs)

    
    FetchContent_Declare(
        implot
        GIT_REPOSITORY https://github.com/epezent/implot.git
        GIT_TAG        "f1b0792cd3e239f615d4f20b9647d37594de8c56"
    )
    FetchContent_MakeAvailable(implot)

    
    FetchContent_Declare(
        implot3d
        GIT_REPOSITORY https://github.com/brenocq/implot3d.git
        GIT_TAG        "4be7ae65624b04cd43bf847f02e4b75e4469cb72"
    )
    FetchContent_MakeAvailable(implot3d)

    
    file(GLOB IMGUI_KNOBS_SOURCES ${imgui_knobs_SOURCE_DIR}/*.cpp)
    add_library(imgui_knobs_lib STATIC ${IMGUI_KNOBS_SOURCES})
    target_include_directories( imgui_knobs_lib PUBLIC ${imgui_knobs_SOURCE_DIR})
    target_link_libraries(      imgui_knobs_lib PRIVATE hello_imgui             )
    set_target_properties(      imgui_knobs_lib PROPERTIES CXX_CLANG_TIDY ""    )
    
    file(GLOB IMPLOT_SOURCES ${implot_SOURCE_DIR}/*.cpp)
    add_library(implot_lib STATIC ${IMPLOT_SOURCES})
    target_include_directories( implot_lib PUBLIC ${implot_SOURCE_DIR} )
    target_link_libraries(      implot_lib PRIVATE hello_imgui         )
    set_target_properties(      implot_lib PROPERTIES CXX_CLANG_TIDY "")
    
    file(GLOB IMPLOT3D_SOURCES ${implot3d_SOURCE_DIR}/*.cpp)
    add_library(implot3d_lib STATIC ${IMPLOT3D_SOURCES})
    target_include_directories( implot3d_lib PUBLIC ${implot3d_SOURCE_DIR})
    target_link_libraries(      implot3d_lib PRIVATE hello_imgui          )
    set_target_properties(      implot3d_lib PROPERTIES CXX_CLANG_TIDY "" )
    
    add_library(sketch_gui_libs INTERFACE)
    target_link_libraries(sketch_gui_libs INTERFACE
        imgui_knobs_lib
        implot_lib
        implot3d_lib
    )
    
endif()
