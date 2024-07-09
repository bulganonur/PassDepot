cmake_minimum_required(VERSION 3.28)

# Project details
project(
  PassDepot
  VERSION 0.1.0
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

# Include
include(CMake/StandardSettings.cmake)
include(CMake/Utils.cmake)
include(CMake/SourcesAndHeaders.cmake)
include(CMake/CompilerWarnings.cmake)

# Include Directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Include/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/External/ImGui/Include/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/External/GLFW/Include/)

# Set project options
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()
message(STATUS "Started CMake for ${PROJECT_NAME} v${PROJECT_VERSION}...\n")

# Prevent building in the source directory
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.\n")
endif()

# Log source files to console
if(${PROJECT_NAME}_VERBOSE_OUTPUT)
  verbose_message("Found the following sources:")
  foreach(source IN LISTS exe_sources)
    verbose_message("* ${source}")
  endforeach()
endif()

# Create executable
add_executable(${PROJECT_NAME} ${exe_sources})

# Set output directories
set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
  # RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)
message(STATUS "Added all header and implementation files.\n")

# Set the project standard and warnings
if(${PROJECT_NAME}_BUILD_HEADERS_ONLY)
  target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)
else()
  target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
endif()
set_project_warnings(${PROJECT_NAME})
verbose_message("Applied compiler warnings. Using standard ${CMAKE_CXX_STANDARD}.\n")



#
# Link with the libraries the project uses
#

# target_link_libraries -> Specify libraries to use when linking a given target
# include_directories -> Add the given directories to those the compiler (probably preprocessor) uses to search for include files.

# GLFW
target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/External/GLFW/lib-vc2022/glfw3.lib)

# OpenGL
target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/External/OpenGL/Lib/OpenGL32.lib)

# # GLM
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/glm/include/)

verbose_message("Successfully added all dependencies and linked against them.")

message(STATUS "Finished building requirements for installing the package.\n")
