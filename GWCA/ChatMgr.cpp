#include "ChatMgr.h"

#include "MemoryMgr.h"
#include "CtoSMgr.h"
#include "PatternScanner.h"
#include <sstream>
#include <iomanip>

GWAPI::ChatMgr::ChatMgr(GWAPIMgr* parent) : parent_(parent)
{
	BeginHook((BYTE*)det_chatlog, (BYTE*)det_chatcmd);
	SetColor(0xff00); // green
}

void GWAPI::ChatMgr::BeginHook(BYTE* chatlog, BYTE* chatcmd)
{
	PatternScanner scanner(0x401000, 0x4FF000);
	BYTE* chatlog_adr = (BYTE*)scanner.FindPattern("\x53\x56\x8B\xF1\x57\x8B\x56\x14\x8B\x4E\x0C\xE8", "xxxxxxxxxxxx", -6);
	BYTE* chatcmd_adr = (BYTE*)scanner.FindPattern("\x8B\xD1\x68\x8A\x00\x00\x00\x8D\x8D\xE8\xFE\xFF\xFF", "xxxxxxxxxxxxx", -0xC);

	DWORD chatlog_length = Hook::CalculateDetourLength(chatlog_adr);
	DWORD chatcmd_length = Hook::CalculateDetourLength(chatcmd_adr);

	ori_chatlog = (ChatLog_t)hk_chatlog_.Detour(chatlog_adr, chatlog, chatlog_length);
	ori_chatcmd = (ChatCmd_t)hk_chatcmd_.Detour(chatcmd_adr, chatcmd, chatcmd_length);

	hooked_ = true;
}

void GWAPI::ChatMgr::EndHook()
{
	if (!hooked_) return;
	hk_chatlog_.Retour();
	hk_chatcmd_.Retour();

	hooked_ = false;
}

void GWAPI::ChatMgr::SendChat(const wchar_t* msg, wchar_t channel)
{
	static P5E_SendChat* chat = new P5E_SendChat();

	chat->channel = channel;
	wcscpy_s(chat->msg, msg);

	parent_->CtoS()->SendPacket<P5E_SendChat>(chat);
}

void GWAPI::ChatMgr::WriteChatF(const wchar_t* format, ...)
{
	static wchar_t* chat = NULL;


	if (chat != NULL) delete[] chat;

	va_list vl;
	va_start(vl, format);
	size_t szbuf = _vscwprintf(format, vl) + 1;
	chat = new wchar_t[szbuf];
	vswprintf_s(chat, szbuf, format, vl);
	va_end(vl);

	WriteChat(chat);
}

void GWAPI::ChatMgr::WriteChat(const wchar_t* msg, const wchar_t* from) {

	((void(__fastcall *)(DWORD, const wchar_t*, const wchar_t*))
		MemoryMgr::WriteChatFunction)
		(0, from, msg);
}

std::wstring GWAPI::ChatMgr::RemakeMessage(const wchar_t* format, ...)
{
	/*
	%xH, %xM, %xS = hours, minutes, secondes where x is a digit
	%N = name of the sender (unimplemented)
	%T = Text
	%C{string} = color string, color is retrieve from argument
	*/
	va_list args;
	va_start(args, format);
	std::wostringstream buffer;
	DWORD time = GWAPI::GWAPIMgr::instance()->Map()->GetInstanceTime() / 1000;

	while (*format)
	{
		if (*format == '%')
		{
			format++;
			DWORD width = 0;
			while (*format > '0' && *format <= '9')
			{
				width *= 10;
				width = *format - '0';
				format++;
			}
			if (!width) width = 1;

			switch (*format)
			{
				case 'H': buffer << std::dec << std::setw(width) << std::setfill(L'0') << (time / 3600) % 60;	break;
				case 'M': buffer << std::dec << std::setw(width) << std::setfill(L'0') << (time / 60) % 60;		break;
				case 'S': buffer << std::dec << std::setw(width) << std::setfill(L'0') << time % 60;			break;
				case 'C': buffer << "<c=#" << std::hex << std::setw(6) << std::setfill(L'0') << va_arg(args, CHAT_COLOR) << ">"; format++; break;
				case 'N': break;
				case 'T': buffer << va_arg(args, wchar_t*); break;
			}
		}
		else if (*format == '}')
		{
			if (format[1]) buffer << "</c>";
		}
		else
		{
			buffer << *format;
		}
		format++;
	}

	va_end(args);
	return buffer.str();
}

DWORD GWAPI::ChatMgr::getChan(wchar_t* message)
{
	wchar_t* start = wcswcs(message, L"<a=1>c=");
	wchar_t* end = wcswcs(message + sizeof("<a=1>c="), L"</a>");

	if (!start || !end) return 0;

	DWORD channel = 0;
	start += strlen("<a=1>c=");
	while (start < end)
	{
		channel *= 10;
		channel += *start - '0';
		start++;
	}

	return channel;
}
struct MessageInfo {
	WCHAR *message;
	DWORD size1;
	DWORD size2;
	DWORD unknow;
};

struct ChannelInfo {
	DWORD unknow1;
	DWORD channel;
	DWORD isHandled; // seem to be 1 until he is handled
	BYTE unknow2[12];
	DWORD unknow3; // alway 6
	DWORD unknow4;
};

void __fastcall GWAPI::ChatMgr::det_chatlog(DWORD ecx, DWORD edx, DWORD useless /* same as edx */)
{
	GWAPI::ChatMgr *chat = GWAPI::GWAPIMgr::instance()->Chat();
	MessageInfo *mInfo = reinterpret_cast<MessageInfo*>(edx);
	ChannelInfo *cInfo = reinterpret_cast<ChannelInfo*>(ecx);

	DWORD speChannel = chat->getChan(mInfo->message);
	
	chat->chatlog_result = chat->RemakeMessage(L"%C{[%2M:%2S]} %T", chat->timestamp_color, mInfo->message);

	mInfo->message = (WCHAR*)chat->chatlog_result.c_str();
	mInfo->size1 = (mInfo->size2 = chat->chatlog_result.length() + 1);

	return chat->ori_chatlog(ecx, edx, useless);
}

void __fastcall GWAPI::ChatMgr::det_chatcmd(DWORD ecx)
{
	ChatMgr* chat = GWAPI::GWAPIMgr::instance()->Chat();
	WCHAR* message = reinterpret_cast<WCHAR*>(ecx);
	WCHAR channel = *message;

	std::wstring key(message + 1);
	std::wstring arguments(L"");

	DWORD fPos = key.find_first_of(' ');
	if (fPos != std::wstring::npos)
	{
		key = key.substr(0, fPos);
		arguments = std::wstring(message + fPos + 2);
	}

	CallBack callback = chat->chatcmd_callbacks[key];
	if (callback.callback && channel == '/')
	{
		std::vector<std::wstring> args;
		size_t index = 0;
		while (true) {
			size_t pos = arguments.find(L' ', index);

			std::wstring arg;
			if (pos == std::wstring::npos) {
				arg = arguments.substr(index);
			} else {
				arg = arguments.substr(index, pos - index);
			}
			if (!arg.empty()) {
				args.push_back(arg);
			}

			if (pos == std::wstring::npos) break;
			index = pos + 1;
		}

		callback.callback(args);

		if (callback.override)
			return chat->ori_chatcmd((DWORD)L"");
	}

	return chat->ori_chatcmd(ecx);
}
