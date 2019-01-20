#include <assert.h>
#include <stdint.h>
#include <Windows.h>

#include <GWCA/Utilities/Macros.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/GameContainers/Array.h>
#include <GWCA/GameEntities/Position.h>
#include <GWCA/Packets/StoC.h>

#include <GWCA/Managers/ConnMgr.h>

GW::MsgConn * GW::GetGameConn()
{
    static GW::MsgConn** conn = nullptr;
    if (conn == nullptr) {
        auto tmp = Scanner::Find("\x56\x33\xF6\x3B\xCE\x74\x0E", "xxxxxxx", -4);
        if (tmp)
            conn = *(GW::MsgConn***)tmp;
    }
    if (!conn)
        return 0;
    return *conn;
}

void GW::MsgConn::Send(unsigned int len, void * data)
{
    using _Send_impl = void(__fastcall*)(MsgConn*, unsigned int, void*);
    static _Send_impl fn = nullptr;
    if (fn == nullptr) {
        fn = (_Send_impl)Scanner::Find("\x55\x8B\xEC\x83\xEC\x2C\x53\x56\x57\x8B\xF9\x85", "xxxxxxxxxxxx", 0);
    }
    fn(this, len, data);
}

void GW::MsgConn::Send(unsigned int argc, ...)
{
    void* args = (&argc + 1);
    using _Send_impl = void(__fastcall*)(MsgConn*, void*, unsigned int);
    static _Send_impl fn = nullptr;
    if (fn == nullptr) {
        fn = (_Send_impl)Scanner::Find("\x55\x8B\xEC\x83\xEC\x18\x53\x56\x57\x8B\xF9\x85\xFF\x8B\xF2\x75\x14", "xxxxxxxxxxxxxxxxx", 0);
    }
    fn(this, args, argc);
}
