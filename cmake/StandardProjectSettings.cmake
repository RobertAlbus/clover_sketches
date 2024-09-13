# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(
    STATUS "Setting build type to 'RelWithDebInfo' as none was specified.")
  set(CMAKE_BUILD_TYPE
      RelWithDebInfo
      CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui, ccmake
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
                                               "MinSizeRel" "RelWithDebInfo")
endif()

find_program(CLOVER_SKETCHES_CCACHE ccache)
find_program(CLOVER_SKETCHES_SCCACHE sccache)
if(CLOVER_SKETCHES_CCACHE)
    message("using ccache")
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CLOVER_SKETCHES_CCACHE})
else(CLOVER_SKETCHES_SCCACHE)
    message("using sccache")
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CLOVER_SKETCHES_SCCACHE})
endif()

# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Force generation of compile_commands.json" FORCE)


