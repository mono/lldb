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
    message("Unable to find ${MONO_DEBUG_LIB}")
    unset(MONO_DEBUG_LIB)
  endif()

  if (NOT EXISTS ${MONO_DEBUG_DLL})
    message("Unable to find ${MONO_DEBUG_DLL}")
    unset(MONO_DEBUG_DLL)
  endif()

  if (NOT EXISTS ${MONO_RELEASE_LIB})
    message("Unable to find ${MONO_RELEASE_LIB}")
    unset(MONO_RELEASE_LIB)
  endif()

  if (NOT EXISTS ${MONO_RELEASE_LIB})
    message("Unable to find ${MONO_RELEASE_LIB}")
    unset(MONO_RELEASE_LIB)
  endif()

  set(MONO_LIB ${MONO_DEBUG_LIB} PARENT_SCOPE)
  set(MONO_DLL ${MONO_DEBUG_DLL} PARENT_SCOPE)
endfunction(find_mono_libs_windows)

if (NOT LLDB_DISABLE_MONO)
  if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    find_mono_libs_windows()
  endif()

  if (MONO_INCLUDE_DIRS)
    include_directories(${MONO_INCLUDE_DIRS})
  endif()  
endif()

if (LLDB_DISABLE_MONO)
  unset(MONO_INCLUDE_DIRS)
  unset(MONO_LIB)
  add_definitions( -DLLDB_DISABLE_MONO )
endif()
