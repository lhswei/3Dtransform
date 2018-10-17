project(transform)
cmake_minimum_required(VERSION 2.8)

set(TRANSFORM_TARGET "transform")

include_directories(
  ./src
  ../include)

file(GLOB_RECURSE TRANSFORM_SRC_LIST 
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
)

link_directories("${DEMON_LIB}" "${DEMON_LIB}/$<CONFIGURATION>")
link_libraries("opengl32.lib" "glut32.lib" "glu32.lib")
## demon
add_executable(${TRANSFORM_TARGET} ${TRANSFORM_SRC_LIST})

source_group("src" FILES ${TRANSFORM_SRC_LIST})

SET_TARGET_OUT_DIR(TRANSFORM_TARGET_OUT_DIR)
COPY_FILES("${TRANSFORM_TARGET}" "${DEMON_BINARY_FILES}" "${DEMON_BINARY_DIR}" "${TRANSFORM_TARGET_OUT_DIR}")

