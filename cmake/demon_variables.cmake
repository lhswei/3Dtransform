# Must be loaded via FindDEMON.cmake.
if(NOT DEFINED _DEMON_ROOT_EXPLICIT)
  message(FATAL_ERROR "Use find_package(DEMON) to load this file.")
endif()

# Determine the project architecture.
if(NOT DEFINED PROJECT_ARCH)
  if(CMAKE_SIZEOF_VOID_P MATCHES 8)
    set(PROJECT_ARCH "x86_64")
  else()
    set(PROJECT_ARCH "x86")
  endif()
endif()

# Path to the include directory.
set(DEMON_INCLUDE_PATH "${_DEMON_ROOT}/include")

# Shared compiler/linker flags.
list(APPEND DEMON_COMPILER_DEFINES
  # Allow C++ programs to use stdint.h macros specified in the C99 standard that aren't 
  # in the C++ standard (e.g. UINT8_MAX, INT64_MIN, etc)
  __STDC_CONSTANT_MACROS __STDC_FORMAT_MACROS
)

# Consumers who run into LNK4099 warnings can pass /Z7 instead (see issue #385).
set(DEMON_DEBUG_INFO_FLAG "/Zi" CACHE STRING "Optional flag specifying specific /Z flag to use")

# Consumers using different runtime types may want to pass different flags
set(DEMON_RUNTIME_LIBRARY_FLAG "/MT" CACHE STRING "Optional flag specifying which runtime to use")
if (DEMON_RUNTIME_LIBRARY_FLAG)
  list(APPEND DEMON_COMPILER_FLAGS_DEBUG ${DEMON_RUNTIME_LIBRARY_FLAG}d)
  list(APPEND DEMON_COMPILER_FLAGS_RELEASE ${DEMON_RUNTIME_LIBRARY_FLAG})
endif()

# Platform-specific compiler/linker flags.
set(DEMON_LIBTYPE STATIC)

list(APPEND DEMON_COMPILER_FLAGS
  /MP           # Multiprocess compilation
  /Gy           # Enable function-level linking
  /GR-          # Disable run-time type information
  /W4           # Warning level 4
  /WX           # Treat warnings as errors
  /wd4100       # Ignore "unreferenced formal parameter" warning
  /wd4127       # Ignore "conditional expression is constant" warning
  /wd4244       # Ignore "conversion possible loss of data" warning
  /wd4481       # Ignore "nonstandard extension used: override" warning
  /wd4512       # Ignore "assignment operator could not be generated" warning
  /wd4701       # Ignore "potentially uninitialized local variable" warning
  /wd4702       # Ignore "unreachable code" warning
  /wd4996       # Ignore "function or variable may be unsafe" warning
  /wd4457       # Ignore "function or variable may be unsafe" warning
  /wd4458       # Ignore "function or variable may be unsafe" warning
  /wd4238       # Ignore "function or variable may be unsafe" warning
  /wd4101       # Ignore "function or variable may be unsafe" warning
  /wd4003       # Ignore "function or variable may be unsafe" warning
  ${DEMON_DEBUG_INFO_FLAG}
)

list(APPEND DEMON_COMPILER_FLAGS_DEBUG
  /RTC1         # Disable optimizations
  /Od           # Enable basic run-time checks
)
list(APPEND DEMON_COMPILER_FLAGS_RELEASE
  /O2           # Optimize for maximum speed
  /Ob2          # Inline any suitable function
  /GF           # Enable string pooling
)
list(APPEND DEMON_LINKER_FLAGS_DEBUG
  /DEBUG        # Generate debug information
)
list(APPEND DEMON_EXE_LINKER_FLAGS
  /MANIFEST:NO        # No default manifest (see ADD_WINDOWS_MANIFEST macro usage)
  /LARGEADDRESSAWARE  # Allow 32-bit processes to access 3GB of RAM
)
list(APPEND DEMON_COMPILER_DEFINES
  WIN32 _WIN32 _WINDOWS             # Windows platform
  UNICODE _UNICODE                  # Unicode build
  WINVER=0x0601 _WIN32_WINNT=0x601  # Targeting Windows 7
  NOMINMAX                          # Use the standard's templated min/max
  WIN32_LEAN_AND_MEAN               # Exclude less common API declarations
  _HAS_EXCEPTIONS=0                 # Disable exceptions
)
list(APPEND DEMON_COMPILER_DEFINES_RELEASE
  NDEBUG _NDEBUG                    # Not a debug build
)

# directory paths.
set(DEMON_BINARY_DIR          "${_DEMON_ROOT}/$<CONFIGURATION>")
set(DEMON_BINARY_DIR_DEBUG    "${_DEMON_ROOT}/Debug")
set(DEMON_BINARY_DIR_RELEASE  "${_DEMON_ROOT}/Release")

# List binary files.
set(DEMON_BINARY_FILES
  glut32.dll
  )
