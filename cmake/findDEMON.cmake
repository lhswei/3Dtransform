# Find the DEMON binary distribution root directory.
set(_DEMON_ROOT "")
if(DEMON_ROOT AND IS_DIRECTORY "${DEMON_ROOT}")
  set(_DEMON_ROOT "${DEMON_ROOT}")
  set(_DEMON_ROOT_EXPLICIT 1)
else()
  set(_ENV_DEMON_ROOT "")
  if(DEFINED ENV{DEMON_ROOT})
    file(TO_CMAKE_PATH "$ENV{DEMON_ROOT}" _ENV_DEMON_ROOT)
  endif()
  if(_ENV_DEMON_ROOT AND IS_DIRECTORY "${_ENV_DEMON_ROOT}")
    set(_DEMON_ROOT "${_ENV_DEMON_ROOT}")
    set(_DEMON_ROOT_EXPLICIT 1)
  endif()
  unset(_ENV_DEMON_ROOT)
endif()

if(NOT DEFINED _DEMON_ROOT_EXPLICIT)
  message(FATAL_ERROR "Must specify a DEMON_ROOT value via CMake or environment variable.")
endif()

if(NOT IS_DIRECTORY "${_DEMON_ROOT}/cmake")
  message(FATAL_ERROR "No CMake bootstrap found for DEMON binary distribution at: ${DEMON_ROOT}.")
endif()

# Execute additional cmake files from the DEMON binary distribution.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${_DEMON_ROOT}/cmake")
include("demon_variables")
include("demon_macros")