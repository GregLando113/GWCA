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

    enum Channel {
        CHANNEL_ALLIANCE    = 0,
        CHANNEL_ALLIES      = 1,
        CHANNEL_GWCA1       = 2,
        CHANNEL_ALL         = 3,
        CHANNEL_GWCA2       = 4,
        CHANNEL_MODERATOR   = 5,
        CHANNEL_EMOTE       = 6,
        CHANNEL_WARNING     = 7, // shows in the middle of the screen and does not parse <c> tags
        CHANNEL_GWCA3       = 8,
        CHANNEL_GUILD       = 9,
        CHANNEL_GLOBAL      = 10,
        CHANNEL_GROUP       = 11,
        CHANNEL_TRADE       = 12,
        CHANNEL_ADVISORY    = 13,
        CHANNEL_WHISPER     = 14,
        CHANNEL_COUNT
    };

    enum class Event {
        OnTick,

        OnServerMessage,

        OnAgentSpawned,
        OnAgentDespawned,
        OnAgentHealthChange,

        OnItemPickup,

        OnQuestCompleted,

		OnChatMessage,
    };

    GWAPI void RegisterEvent  (HMODULE plugin, Event e);
    GWAPI void UnregisterEvent(HMODULE plugin, Event e);

    GWAPI HMODULE LoadPlugin(CString path);
    GWAPI void UnloadPlugin(HMODULE plugin);

    GWAPI CString GetPluginsDir(void);
    GWAPI DWORD   GetGameTime(void);

    //////////////////////////////
    //           Chat           //
    //////////////////////////////
    GWAPI void SendChat(char channel, const wchar *msg);
    GWAPI void SendChat(char channel, const char  *msg);

    GWAPI void PrintChat(Channel channel, const wchar *msg);
    GWAPI void PrintChat(Channel channel, const wchar *sender, const wchar *msg);

    GWAPI void PrintChat(Channel channel, const char *msg);
    GWAPI void PrintChat(Channel channel, const char *sender, const char *msg);

    typedef void (*CmdCB)(CString cmd, CString args);
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
} // namespace GWCA

#endif // GWCA_API_H
