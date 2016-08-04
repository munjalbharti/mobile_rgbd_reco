# Copyright (c) 2016 Robert Maier
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(TsdfAnalysis C CXX)

# RelWithDebInfo, Release or Debug
IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE RelWithDebInfo)
ENDIF()

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# CMake paths
# Externals directories (from environment variable "EXT")
LIST(APPEND CMAKE_PREFIX_PATH "$ENV{EXT}")
# include files in cmake subfolder
LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

#==============================================================================
# Dependencies
#==============================================================================

# Eigen
FIND_PACKAGE(Eigen REQUIRED)
INCLUDE_DIRECTORIES(${EIGEN_INCLUDE_DIR})

# OpenCV
FIND_PACKAGE(OpenCV 2.4.8 COMPONENTS core highgui imgproc REQUIRED)

#==============================================================================
# C++11 support
#==============================================================================
INCLUDE(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
IF(COMPILER_SUPPORTS_CXX11)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ELSEIF(COMPILER_SUPPORTS_CXX0X)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
ELSE()
    MESSAGE(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
ENDIF()

#==============================================================================
# Application
#==============================================================================

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -funroll-loops")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

IF(CMAKE_COMPILER_IS_GNUCC)
    # Add -O0 to remove optimizations when using gcc
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
    SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

# preprocessor define: source directory
ADD_DEFINITIONS(-DAPP_SOURCE_DIR=${PROJECT_SOURCE_DIR})

# applications
FILE(GLOB incs "src/*.h")
FILE(GLOB srcs "src/*.cc")

ADD_EXECUTABLE(${PROJECT_NAME} ${incs} ${srcs}  )
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${OpenCV_LIBS} )

