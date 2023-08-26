#include "stdafx.h"

#include <GWCA/Utilities/Debug.h>

namespace GW
{
    // @Robustness:
    // Should we create a mutex arround that?

    static LogHandler s_LogHandler;
    static void* s_LogHandlerContext;
    static PanicHandler s_PanicHandler;
    static void* s_PanicHandlerContext;

    void RegisterLogHandler(LogHandler handler, void *context)
    {
        s_LogHandler = handler;
        s_LogHandlerContext = context;
    }

    void RegisterPanicHandler(PanicHandler handler, void *context)
    {
        s_PanicHandler = handler;
        s_PanicHandlerContext = context;
    }

    __declspec(noreturn) void FatalAssert(
        const char *expr,
        const char *file,
        unsigned int line,
        const char *function)
    {
        if (s_PanicHandler) {
            s_PanicHandler(s_PanicHandlerContext, expr, file, line, function);
        }
        abort();
    }

    void __cdecl LogMessage(
        LogLevel level,
        const char *file,
        unsigned int line,
        const char *function,
        const char *fmt,
        ...)
    {
        va_list args;
        va_start(args, fmt);
        GW::LogMessageV(level, file, line, function, fmt, args);
        va_end(args);
    }

    void __cdecl LogMessageV(
        LogLevel level,
        const char *file,
        unsigned int line,
        const char *function,
        const char *fmt,
        va_list args)
    {
        if (s_LogHandler == nullptr)
            return;

        char message[1024];
        vsnprintf(message, sizeof(message), fmt, args);
        s_LogHandler(s_LogHandlerContext, level, message, file, line, function);
    }
}
