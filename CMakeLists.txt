cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project( PCL_Implementation )

find_package( OpenCV REQUIRED )

#####--------------------------
##### PCL
#####--------------------------
find_package( PCL REQUIRED )
message (STATUS "")
if (PCL_FOUND)
  message (STATUS "")
  message (STATUS " -----------------------")
  message (STATUS " Found PCL version ${PCL_VERSION}")
  message (STATUS "     * include dirs: ${PCL_INCLUDE_DIRS}")
  message (STATUS "     * library dirs: ${PCL_LIBRARY_DIRS}")
  message (STATUS "     * libraries:    ${PCL_LIBRARIES}")
else()
  message (FATAL_ERROR : "PCL NOT FOUND!")
endif()
message (STATUS "")

# Add project cmake modules to path.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

# We probably don't want this to run on every build.
option(COVERAGE "Generate Coverage Data" OFF)

if (COVERAGE)
    include(CodeCoverage)
    set(LCOV_REMOVE_EXTRA "'vendor/*'" "'include/Eigen/*'" "'opencv-3.3.0/*'")
    setup_target_for_coverage(code_coverage test/cpp-test coverage)
    set(COVERAGE_SRCS src/main.cpp)

    SET(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
    SET(CMAKE_C_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
    SET(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")
else()
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -g")
endif()

include(CMakeToolsHelpers OPTIONAL)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 11)

add_subdirectory(src)
# add_subdirectory(test)
# add_subdirectory(vendor/googletest/googletest)