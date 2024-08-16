cmake_minimum_required(VERSION 3.25)

# Set CMAKE_TOOLCHAIN_FILE before project()
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

# Project details
project(
  PassDepot
  VERSION 0.1.0
  LANGUAGES CXX
)
  
set(CMAKE_CXX_STANDARD 20)
  
# Include CMake files
include(CMake/StandardSettings.cmake)
include(CMake/Utils.cmake)
include(CMake/SourcesAndHeaders.cmake)
include(CMake/CompilerWarnings.cmake)
  
# Include Directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Include/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/External/DearImGui/Include/)

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

# On Windows, build a GUI application
if(WIN32 AND ${PROJECT_NAME}_BUILD_EXECUTABLE AND ${PROJECT_NAME}_BUILD_GUI)
  set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

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
# Link against libraries
#

# fmt
find_package(fmt CONFIG REQUIRED)

# OpenGL
find_package(OpenGL REQUIRED)

# GLFW
find_package(glfw3 CONFIG REQUIRED)

# LibSodium
find_package(unofficial-sodium CONFIG REQUIRED)

# sqlite3
find_package(unofficial-sqlite3 CONFIG REQUIRED)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt OpenGL::GL glfw unofficial-sodium::sodium unofficial::sqlite3::sqlite3)

# For Windows, it is necessary to link with the MultiThreaded library.
# Depending on how the rest of the project's dependencies are linked, it might be necessary
# to change the line to statically link with the library.
#
# This is done as follows:
#
# set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
# set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
#
# On Linux and Mac this variable is ignored. If any issues rise from it, try commenting it out
# and letting CMake decide how to link with it.

verbose_message("Successfully added all dependencies and linked against them.\n")

message(STATUS "Finished building requirements for installing the package.\n")
