cmake_minimum_required(VERSION 2.8)

# Only generate Debug and Release configuration types.
set(CMAKE_CONFIGURATION_TYPES Debug Release)
# Project name.
# TODO: Change this line to match your project name when you copy this file.
project(demon)
# Use folders in the resulting project files.
set_property(GLOBAL PROPERTY OS_FOLDERS ON)

if(CMAKE_CL_64)    #CMAKE的内建变量，如果是true，就说明编译器的64位的，自然可以编译64bit的程序
  set(ADDRESS_MODEL 64)
  set(NODE_TARGET x64)
else()
  set(ADDRESS_MODEL 32)
  set(NODE_TARGET x86)
endif()

set(DEMON_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${DEMON_ROOT}/cmake")
set(DEMON_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include" )
set(DEMON_LIB "${CMAKE_CURRENT_SOURCE_DIR}/lib")

# Execute FindDEMON.cmake which must exist in CMAKE_MODULE_PATH.
find_package(DEMON REQUIRED)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/transform")
  add_subdirectory(transform)
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/projection" )
  add_subdirectory(projection)
endif()

if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(main PRIVATE "-std=c++11")
    MESSAGE(STATUS "change complie to c++11")  
endif()
