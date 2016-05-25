add_lldb_library(lldbPluginScriptInterpreterMono
  ScriptInterpreterMono.cpp
  )

target_include_directories(lldbPluginScriptInterpreterMono
  PUBLIC ${MONO_DIRECTORY}/msvc/include)