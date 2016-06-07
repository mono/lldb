function(find_mono_libs_windows)  
  if ("${MONO_HOME}" STREQUAL "")
    message("LLDB embedded Mono on Windows requires specifying a value for MONO_HOME.  Mono support disabled.")
    set(LLDB_DISABLE_MONO 1 PARENT_SCOPE)
    return()
  endif()

  message("-- Found Mono: ${MONO_HOME}")

  file(TO_CMAKE_PATH "${MONO_HOME}" MONO_HOME)

  file(TO_CMAKE_PATH "${MONO_HOME}/msvc/include" MONO_INCLUDE_DIRS)
  set(MONO_INCLUDE_DIRS "${MONO_INCLUDE_DIRS}" PARENT_SCOPE)

  set(MONOLIBS_BASE_NAME monosgen-2.0)

  file(TO_CMAKE_PATH "${MONO_HOME}/msvc/Win32/lib/Debug_SGen/${MONOLIBS_BASE_NAME}.lib" MONO_DEBUG_LIB)
  file(TO_CMAKE_PATH "${MONO_HOME}/msvc/Win32/bin/Debug_SGen/${MONOLIBS_BASE_NAME}.dll" MONO_DEBUG_DLL)
  file(TO_CMAKE_PATH "${MONO_HOME}/msvc/Win32/lib/Release_SGen/${MONOLIBS_BASE_NAME}.lib" MONO_RELEASE_LIB)
  file(TO_CMAKE_PATH "${MONO_HOME}/msvc/Win32/bin/Release_SGen/${MONOLIBS_BASE_NAME}.lib" MONO_RELEASE_DLL)

  if (NOT EXISTS ${MONO_DEBUG_LIB})
    message(FATAL_ERROR "Unable to find Mono debug library")
    unset(MONO_DEBUG_LIB)
  endif()

  if (NOT EXISTS ${MONO_DEBUG_DLL})
    message(FATAL_ERROR "Unable to find Mono debug DLL")
    unset(MONO_DEBUG_DLL)
  endif()

  if (NOT EXISTS ${MONO_RELEASE_LIB})
    message(FATAL_ERROR "Unable to find Mono release library")
    unset(MONO_RELEASE_LIB)
  endif()

  if (NOT EXISTS ${MONO_RELEASE_DLL})
    message(FATAL_ERROR "Unable to find Mono release DLL")
    unset(MONO_RELEASE_DLL)
  endif()

  set(MONO_LIBRARIES ${MONO_DEBUG_LIB} PARENT_SCOPE)
  set(MONO_DLL ${MONO_DEBUG_DLL} PARENT_SCOPE)
endfunction(find_mono_libs_windows)

# - Try to find Mono
# Once done this will define
#  MONO_FOUND - System has Mono
#  MONO_INCLUDE_DIRS - The Mono include directories
#  MONO_LIBRARY_DIRS - The Mono library directories
#  MONO_LIBRARIES - The libraries needed to use Mono
#  MONO_DEFINITIONS - Compiler switches required for using Mono
function(find_mono_libs)
  include(CMakeFindFrameworks)
  cmake_find_frameworks(Mono)

  if(Mono_FRAMEWORKS)
    list(GET Mono_FRAMEWORKS 0 MONO_FRAMEWORK)
    set(MONO_PKG_CONFIG ${MONO_FRAMEWORK}/Commands/pkg-config)
    set(PKG_CONFIG_EXECUTABLE ${MONO_PKG_CONFIG})
    set(PKG_CONFIG_FOUND TRUE)
  endif(Mono_FRAMEWORKS)

  find_package(PkgConfig)
  pkg_check_modules(MONO REQUIRED mono-2)

  set(MONO_INCLUDE_DIRS ${MONO_INCLUDE_DIRS} PARENT_SCOPE)
  set(MONO_LIBRARY_DIRS ${MONO_LIBRARY_DIRS} PARENT_SCOPE)
  set(MONO_LIBRARIES ${MONO_LIBRARIES} PARENT_SCOPE)
endfunction(find_mono_libs)

if (NOT LLDB_DISABLE_MONO)
  if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    find_mono_libs_windows()
  else ()
    find_mono_libs()
  endif()

  if (NOT EXISTS ${MONO_INCLUDE_DIRS})
    message(FATAL_ERROR "Unable to find Mono include directories")
    unset(MONO_INCLUDE_DIRS)
  endif()

  if (NOT EXISTS ${MONO_LIBRARY_DIRS})
    message(FATAL_ERROR "Unable to find Mono library directories")
    unset(MONO_LIBRARY_DIRS)
  endif()
endif()

if (LLDB_DISABLE_MONO)
  unset(MONO_INCLUDE_DIRS)
  unset(MONO_LIBRARY_DIRS)
  unset(MONO_LIBRARIES)
  add_definitions( -DLLDB_DISABLE_MONO )
endif()
