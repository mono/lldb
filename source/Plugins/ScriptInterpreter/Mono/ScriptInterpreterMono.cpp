//===-- ScriptInterpreterMono.cpp -------------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "ScriptInterpreterMono.h"
#include "lldb/Core/Debugger.h"
#include "lldb/Core/PluginManager.h"
#include "lldb/Core/Stream.h"
#include "lldb/Core/StreamFile.h"
#include "lldb/Core/StringList.h"
#include "lldb/Interpreter/CommandInterpreter.h"
#include <mutex>

using namespace lldb;
using namespace lldb_private;

ScriptInterpreterMono::ScriptInterpreterMono(CommandInterpreter &interpreter)
    : ScriptInterpreter(interpreter, eScriptLanguageMono)
{
}

ScriptInterpreterMono::~ScriptInterpreterMono()
{
}

bool
ScriptInterpreterMono::ExecuteOneLine(const char *command, CommandReturnObject *,
    const ExecuteScriptOptions &)
{
    m_interpreter.GetDebugger().GetErrorFile()->PutCString(
        "error: there is no embedded script interpreter in this mode.\n");
    return false;
}

void
ScriptInterpreterMono::ExecuteInterpreterLoop()
{
    m_interpreter.GetDebugger().GetErrorFile()->PutCString(
        "error: there is no embedded script interpreter in this mode.\n");
}

void
ScriptInterpreterMono::Initialize()
{
    static std::once_flag g_once_flag;

    std::call_once(g_once_flag, []()
    {
        PluginManager::RegisterPlugin(GetPluginNameStatic(),
            GetPluginDescriptionStatic(),
            lldb::eScriptLanguageMono,
            CreateInstance);
    });
}

void
ScriptInterpreterMono::Terminate()
{
    // Shutdown Mono runtime
    // mono_jit_cleanup();
}

lldb::ScriptInterpreterSP
ScriptInterpreterMono::CreateInstance(CommandInterpreter &interpreter)
{
    return std::make_shared<ScriptInterpreterMono>(interpreter);
}

lldb_private::ConstString
ScriptInterpreterMono::GetPluginNameStatic()
{
    static ConstString g_name("script-mono");
    return g_name;
}

const char *
ScriptInterpreterMono::GetPluginDescriptionStatic()
{
    return "Mono script interpreter";
}

lldb_private::ConstString
ScriptInterpreterMono::GetPluginName()
{
    return GetPluginNameStatic();
}

uint32_t
ScriptInterpreterMono::GetPluginVersion()
{
    return 1;
}
