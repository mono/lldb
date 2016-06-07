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
#include "lldb/Core/Timer.h"
#include "lldb/Interpreter/CommandInterpreter.h"
#include "llvm/Support/Path.h"
#include "llvm/ADT/SmallString.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/debug-helpers.h>

#include <mutex>

using namespace lldb;
using namespace lldb_private;

static bool g_initialized = false;

ScriptInterpreterMono::ScriptInterpreterMono(CommandInterpreter &interpreter)
    : ScriptInterpreter(interpreter, eScriptLanguageMono)
{
    InitializeMono();
}

ScriptInterpreterMono::~ScriptInterpreterMono()
{
    ShutdownMono();
}

void ScriptInterpreterMono::InitializeMono()
{
    if (g_initialized)
        return;

    g_initialized = true;

    mono_config_parse(nullptr);

    monoDomain = mono_jit_init_version("lldb", "v4.0.30319");

    const char *monoRootDir = mono_assembly_getrootdir();

    llvm::SmallString<256> lldbAssemblyPath;
    llvm::sys::path::append(lldbAssemblyPath, monoRootDir, "LLDBSharp.dll");
    lldbAssembly = mono_domain_assembly_open(monoDomain, lldbAssemblyPath.c_str());

    if (!lldbAssembly) {
        m_interpreter.GetDebugger().GetErrorFile()->PutCString(
            "error: could not find LLDBSharp assembly in the path.\n");
    }

    lldbImage = mono_assembly_get_image(lldbAssembly);

    lldbDebuggerClass = mono_class_from_name(lldbImage, "LLDB", "Debugger");

    if (!lldbDebuggerClass) {
        m_interpreter.GetDebugger().GetErrorFile()->PutCString(
            "error: could not find LLDB.Debugger class in assembly.\n");
    }

    //lldbDebuggerInstance = mono_object_new(monoDomain, lldbDebuggerClass);
    //mono_runtime_object_init(lldbDebuggerInstance);

    // Find the C++# generated __CreateInstance method.
    auto method = std::string(mono_class_get_namespace(lldbDebuggerClass)) + "."
        + std::string(mono_class_get_name(lldbDebuggerClass)) + ":"
        + std::string("__CreateInstance") + "(intptr,bool)";

    MonoMethodDesc* desc = mono_method_desc_new(method.c_str(), true);

    MonoMethod* lldbDebuggerCreateInstanceMethod =
        mono_method_desc_search_in_class(desc, lldbDebuggerClass);

    mono_method_desc_free(desc);

    void* args[2];

    // Setup the parameters to pass.
    // static Debugger __CreateInstance(global::System.IntPtr native, bool skipVTables = false)
    Debugger &debugger = GetCommandInterpreter().GetDebugger();
    args[0] = &debugger;
    args[1] = 0;

    MonoObject *result;
    mono_runtime_invoke(lldbDebuggerCreateInstanceMethod, &result, args, nullptr);
}

void ScriptInterpreterMono::ShutdownMono()
{
    mono_jit_cleanup(monoDomain);
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
    Timer scoped_timer(__PRETTY_FUNCTION__, __PRETTY_FUNCTION__);

    Debugger &debugger = GetCommandInterpreter().GetDebugger();

    if (!debugger.GetInputFile()->GetFile().IsValid())
        return;

    // We do not support loop execution yet in the Mono plugin.
    return;
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
