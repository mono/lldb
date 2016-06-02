//===-- ScriptInterpreterMono.h ---------------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef liblldb_ScriptInterpreterMono_h_
#define liblldb_ScriptInterpreterMono_h_

// C Includes
// C++ Includes
// Other libraries and framework includes
// Project includes
#include "lldb/Interpreter/ScriptInterpreter.h"

typedef struct _MonoDomain MonoDomain;
typedef struct _MonoAssembly MonoAssembly;

namespace lldb_private
{

    class ScriptInterpreterMono : public ScriptInterpreter
    {
        MonoDomain *monoDomain;
        MonoAssembly *lldbAssembly;
        MonoImage *lldbImage;

        void InitializeMono();
        void ShutdownMono();

    public:
        ScriptInterpreterMono(CommandInterpreter &interpreter);

        ~ScriptInterpreterMono() override;

        bool
            ExecuteOneLine(const char *command, CommandReturnObject *result,
                const ExecuteScriptOptions &options = ExecuteScriptOptions()) override;

        void
            ExecuteInterpreterLoop() override;

        //------------------------------------------------------------------
        // Static Functions
        //------------------------------------------------------------------
        static void
            Initialize();

        static void
            Terminate();

        static lldb::ScriptInterpreterSP
            CreateInstance(CommandInterpreter &interpreter);

        static lldb_private::ConstString
            GetPluginNameStatic();

        static const char *
            GetPluginDescriptionStatic();

        //------------------------------------------------------------------
        // PluginInterface protocol
        //------------------------------------------------------------------
        lldb_private::ConstString
            GetPluginName() override;

        uint32_t
            GetPluginVersion() override;
    };

} // namespace lldb_private

#endif // liblldb_ScriptInterpreterMono_h_
