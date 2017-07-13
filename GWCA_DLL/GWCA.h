#ifndef GWCA_API_H
#define GWCA_API_H

#include <stdint.h>


#include <Windows.h>
#include <string>
#include <vector>

// @TODO, fix thoses includes.
#include <GWCA\GWStructures.h>
#include <GWCA\Constants\Constants.h>
#include <GWCA\Constants\Skills.h>
#include <GWCA\Constants\Maps.h>

#define GWCA_VERSION_MAJOR (36604)
#define GWCA_VERSION_MINOR (0)

#define DllExport __declspec(dllexport)
#define DllImport __declspec(dllimport)

#if defined(GWCA_LIB)
#  if defined(GWCA_BUILD_DLL)
#    define GWAPI DllExport
#  else
#    define GWAPI extern
#  endif
#else
#  define GWAPI DllImport
#endif

namespace GWCA {
    using wchar = wchar_t;
    using Color = uint32_t;

    template <typename T>
    using Array = std::vector<T>;

    using CString = std::wstring;

    struct Vec2f { float x, y; };
    struct Vec3f { float x, y, z; };

    Color COLOR_ARGB(int a, int r, int g, int b) {
        Color col = ((a & 0xff) << 24) |
                    ((r & 0xff) << 16) |
                    ((g & 0xff) << 8)  | (b & 0xff);
        return col;
    }

    Color COLOR_RGB(int r, int g, int b) { return COLOR_ARGB(0xff, r, g, b); }

    enum class Channel {
        Alliance    = 0,
        Allies      = 1,
        GWCA1       = 2,
        All         = 3,
        GWCA2       = 4,
        Moderator   = 5,
        Emote       = 6,
        Warning     = 7, // shows in the middle of the screen and does not parse <c> tags
        GWCA3       = 8,
        Guild       = 9,
        Global      = 10,
        Group       = 11,
        Trade       = 12,
        Advisory    = 13,
        Whisper     = 14,

        Count,
    };
    constexpr int CHANNEL_COUNT = (int)Channel::Count;

    enum class Event {
        OnTick,
        OnPacketReceived,

        OnMessageReceived,

        Count
    };
    constexpr int EVENT_COUNT = (int)Event::Count;

    GWAPI void RegisterEvent  (HMODULE plugin, Event e);
    GWAPI void UnregisterEvent(HMODULE plugin, Event e);

    GWAPI HMODULE LoadPlugin(CString path);
    GWAPI void UnloadPlugin(HMODULE plugin);

    GWAPI CString GetPluginsDir(void);
    GWAPI DWORD   TimeGetMs(void);

    //////////////////////////////
    //           Chat           //
    //////////////////////////////
    GWAPI void SendChat(char channel, const wchar *msg);
    GWAPI void SendChat(char channel, const char  *msg);

    GWAPI void PrintChat(Channel channel, const wchar *msg);
    GWAPI void PrintChat(Channel channel, const wchar *sender, const wchar *msg);

    GWAPI void PrintChat(Channel channel, const char *msg);
    GWAPI void PrintChat(Channel channel, const char *sender, const char *msg);

    // @Cleanup, CmdCB shouldn't return something. We have to change that in ChatMgr.
    typedef bool (*CmdCB)(CString cmd, CString args);
    GWAPI void CreateCommand(CString cmd, CmdCB callback);
    GWAPI void DeleteCommand(CString cmd);

    GWAPI void ChangeChatColor(Channel channel, Color sender, Color msg);

    //////////////////////////////
    //          Agent           //
    //////////////////////////////
    GWAPI GW::Agent *GetPlayer();
    GWAPI GW::Agent *GetTarget();
    GWAPI GW::Agent *GetAgentByID(DWORD id);

    GWAPI DWORD GetPlayerId();
    GWAPI DWORD GetTargetId();
} // namespace GW

#endif // GWCA_API_H
