add_lldb_library(lldbPluginScriptInterpreterMono
  ScriptInterpreterMono.cpp
  )

include(${LLDB_PROJECT_ROOT}/cmake/Mono.cmake)

target_include_directories(lldbPluginScriptInterpreterMono
  PUBLIC ${MONO_INCLUDE_DIRS})