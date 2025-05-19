if(CLOVER_SKETCHES_FETCH_DEPENDENCIES)

    include(FetchContent)
    FetchContent_Declare(
        eigen3
        GIT_SHALLOW TRUE
        GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
        GIT_TAG 3.4
    )
    FetchContent_MakeAvailable(eigen3)

endif()
