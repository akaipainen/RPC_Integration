cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

# Refuse to compile against a wrong ROOT version
set(ROOT_VERSION_MIN_5 5.99.99)
set(ROOT_VERSION_MIN_6 6.18.00)

foreach(p CMP0005  # Properly escape preprocessor definitions (v2.6, v3.0.2)
          CMP0025  # Compiler id for Apple Clang is now AppleClang (v3.0)
          CMP0042  # MACOSX_RPATH is enabled by default (v3.0)
          CMP0053  # Simplify variable reference and escape sequence evaluation (v3.1.3)
          CMP0068  # RPATH settings on macOS do not affect install_name (v3.9 and newer)
       )
if(POLICY ${p})
    cmake_policy(SET ${p} NEW)
  endif()
endforeach()

project(RpcIA CXX) # Can add C if using C code

message(STATUS "CMake platform: ${CMAKE_SYSTEM}")
message(STATUS "Build folder: ${RpcIA_BINARY_DIR}")
message(STATUS "Source folder: ${RpcIA_SOURCE_DIR}")
message(STATUS "Installation folder: ${CMAKE_INSTALL_PREFIX}")

# CMake supports different build types by default. We want the
# DEBUG build type to have "-g -O0" flags. By default it only has "-g"
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG -D__DEBUG")

# You can change the build type using
# cmake -DCMAKE_BUILD_TYPE=DEBUG | RELEASE | RELWITHDEBINFO ...
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RELWITHDEBINFO)
endif(NOT CMAKE_BUILD_TYPE)
string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE)
set(CMAKE_ALLOWED_BUILD_TYPES DEBUG RELEASE RELWITHDEBINFO)
list(FIND CMAKE_ALLOWED_BUILD_TYPES "${CMAKE_BUILD_TYPE}" VALID_BUILD_TYPE)
if(${VALID_BUILD_TYPE} EQUAL -1)
    string(REPLACE ";" ", " CMAKE_ALLOWED_BUILD_TYPES_FLAT "${CMAKE_ALLOWED_BUILD_TYPES}")
    message(FATAL_ERROR "Invalid build type ${CMAKE_BUILD_TYPE}. Use one of: ${CMAKE_ALLOWED_BUILD_TYPES_FLAT}.")
endif(${VALID_BUILD_TYPE} EQUAL -1)
message(STATUS "Build type: ${CMAKE_BUILD_TYPE} (${CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}})")

if(CMAKE_INSTALL_PREFIX STREQUAL "${RpcIA_SOURCE_DIR}")
    message(FATAL_ERROR "Please choose a different installation point than the source tree.")
endif(CMAKE_INSTALL_PREFIX STREQUAL "${RpcIA_SOURCE_DIR}")
if(NOT CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX "${RpcIA_SOURCE_DIR}/install")
endif(NOT CMAKE_INSTALL_PREFIX)
message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")

set(LD_LIBRARY_PATH ${CMAKE_INSTALL_PREFIX}/lib:$ENV{LD_LIBRARY_PATH})


# Path to additional modules
set(CMAKE_MODULE_PATH "${RpcIA_SOURCE_DIR}/cmake")

# Check some compiler flags
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-Wno-error=misleading-indentation CC_WNOERROR_MISLEADING_INDENTATION)

# Shared library suffix for non windows
if(NOT CMAKE_SYSTEM_NAME STREQUAL Windows)
    set(CMAKE_SHARED_LIBRARY_SUFFIX .so)    
endif(NOT CMAKE_SYSTEM_NAME STREQUAL Windows)

# Set static build on
# set(RPCIA_STATIC true)

# ROOT configuration mandatory
# Setting variables in cache to be accessible by ccmake
set(ROOTSYS CACHE STRING "ROOT installation folder")
if(ROOTSYS)
    find_package(ROOT REQUIRED)

    if(ROOT_VERSION_MAJOR EQUAL 5)
        set(ROOT_VERSION_MIN "${ROOT_VERSION_MIN_5}")
    elseif(ROOT_VERSION_MAJOR EQUAL 6)
        set(ROOT_VERSION_MIN "${ROOT_VERSION_MIN_6}")
    else()
        message(FATAL_ERROR "RpcIA works with ROOT 5 or 6 only")
    endif()

    if(ROOT_VERSION_NORM VERSION_LESS "${ROOT_VERSION_MIN}")
        message(FATAL_ERROR "RpcIA requires at least ROOT ${ROOT_VERSION_MIN} (you have ${ROOT_VERSION_NORM})")
    endif(ROOT_VERSION_NORM VERSION_LESS "${ROOT_VERSION_MIN}")

else(ROOTSYS)
    message(FATAL_ERROR "ROOT installation not found.\nPlease point to the ROOT installation using -DROOTSYS=ROOT_INSTALL_DIR")
endif(ROOTSYS)

# Enable C++11 by default if found in ROOT
if(ROOT_HAS_CXX11 AND NOT DISABLE_CXX11)
    message(STATUS "Enabling C++11")
    # if(NOT ${CMAKE_CXX_FLAGS} MATCHES "-std=c\\+\\+11")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    # endif(NOT ${CMAKE_CXX_FLAGS} MATCHES "-std=c\\+\\+11")
endif(ROOT_HAS_CXX11 AND NOT DISABLE_CXX11)

# Enable C++14 by default if found in ROOT
if(ROOT_HAS_CXX14 AND NOT DISABLE_CXX14)
    message(STATUS "Enabling C++14")
    # message("Current flags: ${CMAKE_CXX_FLAGS}")
    # if(NOT ${CMAKE_CXX_FLAGS} MATCHES "-std=c\\+\\+14")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
    # endif(NOT ${CMAKE_CXX_FLAGS} MATCHES "-std=c\\+\\+14")
    # message("Current flags: ${CMAKE_CXX_FLAGS}")
endif(ROOT_HAS_CXX14 AND NOT DISABLE_CXX14)

# ROOT dictionaries and maps
include(CMakeRPCIA)

# Generating the RpcIA-config.cmake file
configure_file(${PROJECT_SOURCE_DIR}/cmake/RpcIA-config.cmake.in ${CMAKE_BINARY_DIR}/version/RpcIA-config.cmake @ONLY)
install(FILES ${PROJECT_BINARY_DIR}/version/RpcIA-config.cmake DESTINATION etc)

# General flags -> Should be moved into a configuration file
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
# Avoid problems with -fPIE (set automatically by the previous line).
set(CMAKE_CXX_COMPILE_OPTIONS_PIE "")
set(CMAKE_C_COMPILE_OPTIONS_PIE "")
set(CMAKE_Fortran_COMPILE_OPTIONS_PIE "")
set(CMAKE_NO_SYSTEM_FROM_IMPORTED TRUE)

# Be sure about where libraries and binaries are put
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")

# Build targets with install rpath on Mac to dramatically speed up installation
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
if("${isSystemDir}" STREQUAL "-1")
  if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(CMAKE_INSTALL_RPATH "@loader_path/../lib")
  endif()
endif()
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
else()
  set(CMAKE_SKIP_BUILD_RPATH FALSE)
  set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
  if("${isSystemDir}" STREQUAL "-1")
    SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
  endif("${isSystemDir}" STREQUAL "-1")
endif()
unset(isSystemDir)

# add_subdirectory(3rdParty)
add_subdirectory(BASE)
add_subdirectory(MAPPING)
add_subdirectory(RECON)
add_subdirectory(RAW)
add_subdirectory(ANALYSIS)
add_subdirectory(TASK)
add_subdirectory(executables)
