cmake_minimum_required(VERSION 2.6)

# make the falling2d library
project(falling2d)
set(LIBRARY_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE}/)
file(
  GLOB_RECURSE
  falling2d_lib_sources
  Falling/*
  )
add_library(
  falling2d
  SHARED
  ${falling2d_lib_sources}
  )

# to make the demo application
project(falling_demo)
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE}/)
include_directories(Falling ../SFMLincludes FallingDemo/Box2d)
link_directories(${PROJECT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/)
file(
  GLOB_RECURSE
  falling_demo_exe_sources
  FallingDemo/*
  )
add_executable(
  falling_demo
  ${falling_demo_exe_sources}
  )
target_link_libraries(
  falling_demo
  falling2d
  sfml-system
  sfml-window
  sfml-graphics
  )

