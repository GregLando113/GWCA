#pragma once

#include <Windows.h>
#include "GWStructPositions.h"

/*
Server to client packets, sorted by header

most packets are not filled in, however the list and types of 
fields is given as comments

feel free to fill packets, and you can also add a suffix to 
the packet name, e.g. P391 -> P391_InstanceLoadMap
*/

namespace GWCA {

	namespace StoC {
		struct PacketBase {
			DWORD header;
		};

		template <class Specific>
		struct Packet : PacketBase {
		public:
			static const DWORD STATIC_HEADER;
		};


		struct P000 : Packet<P000> {
			// DWORD
		};
		const DWORD Packet<P000>::STATIC_HEADER = 0;

		struct P001_PingRequest : Packet<P001_PingRequest> {
		};
		const DWORD Packet<P001_PingRequest>::STATIC_HEADER = 1;

		struct P002_PingReply : Packet<P002_PingReply> {
			DWORD data;
		};
		const DWORD Packet<P002_PingReply>::STATIC_HEADER = 2;

		struct P003 : Packet<P003> {
			// DWORD
			// DWORD
			// wchar_t array[256] // prefixType="int16"
			// wchar_t array[256] // prefixType="int16"
		};
		const DWORD Packet<P003>::STATIC_HEADER = 3;

		struct P004 : Packet<P004> {
			// WORD
			// WORD
			// WORD
		};
		const DWORD Packet<P004>::STATIC_HEADER = 4;

		struct P005 : Packet<P005> {
			// BYTE
		};
		const DWORD Packet<P005>::STATIC_HEADER = 5;

		struct P006 : Packet<P006> {
			// WORD
		};
		const DWORD Packet<P006>::STATIC_HEADER = 6;

		struct P007 : Packet<P007> {
			// WORD
		};
		const DWORD Packet<P007>::STATIC_HEADER = 7;

		struct P008 : Packet<P008> {
			// BYTE
		};
		const DWORD Packet<P008>::STATIC_HEADER = 8;

		struct P009 : Packet<P009> {
			// BYTE
			// DWORD
			// DWORD
		};
		const DWORD Packet<P009>::STATIC_HEADER = 9;

		struct P010 : Packet<P010> {
			// BYTE
			// DWORD
		};
		const DWORD Packet<P010>::STATIC_HEADER = 10;

		struct P011 : Packet<P011> {
			// wchar_t array[122] // prefixType="int16"
		};
		const DWORD Packet<P011>::STATIC_HEADER = 11;

		struct P012 : Packet<P012> {
			// BYTE
			// BYTE
			// DWORD
			// DWORD
			// BYTE
		};
		const DWORD Packet<P012>::STATIC_HEADER = 12;

		struct P013 : Packet<P013> {
			// int[8] // prefixType="int16"
		};
		const DWORD Packet<P013>::STATIC_HEADER = 13;

		struct P014 : Packet<P014> {
			// WORD
			// BYTE
			// <Field occurs="64" prefixType="int8" type="nested">
			// DWORD
		};
		const DWORD Packet<P014>::STATIC_HEADER = 14;

		struct P015 : Packet<P015> {
			// WORD
			// <Field occurs="64" prefixType="int8" type="nested">
			// DWORD
		};
		const DWORD Packet<P015>::STATIC_HEADER = 15;

		struct P016 : Packet<P016> {
		};
		const DWORD Packet<P016>::STATIC_HEADER = 16;

		struct P017 : Packet<P017> {
			// WORD
			// BYTE
		};
		const DWORD Packet<P017>::STATIC_HEADER = 17;

		struct P018 : Packet<P018> {
			// int[128] // prefixType="int16"
		};
		const DWORD Packet<P018>::STATIC_HEADER = 18;

		// Heartbeat
		struct P019 : Packet<P019> {
			// DWORD
		};
		const DWORD Packet<P019>::STATIC_HEADER = 19;

		struct P020 : Packet<P020> {
			// DWORD
		};
		const DWORD Packet<P020>::STATIC_HEADER = 20;

		// spawn object
		struct P021 : Packet<P021> {
			DWORD agent_id;
			DWORD data1;	// for agents, this most likely pointer to the 3d file ID
							// for items, this is a "Local ID" that was sent by P343 or P344
			DWORD data2;		// 1 moving agents, 2 signposts, 3 chests [?], 4 items
			DWORD data3;		// 0 items, 1 signposts, 9 agents
			GW::GamePos pos;
			float rotation1;//0x0020 those 2 values are both between -1 and 1, and look like 
			float rotation2;//0x0024 rotations. Maybe cos/sin? Idk, but one of them is definitely rotation.
			DWORD unk2;		//0x0028 
			float speed;	//0x002C 
			float unk3;		//0x0030 
			float unk4;		//0x0034 0 items, agents 12, signposts 20
			DWORD unk5;		//0x0038 zero for items, weird big value for agents
			DWORD unk6;		//0x003C 
			DWORD unk7;		//0x0040 
			DWORD unk8;		//0x0044 
			DWORD unk9;		//0x0048 
			DWORD unk10;	//0x004C 
			float unk11;	//0x0050 a value between -100 and 100 for agents, Inf for items
			float unk12;	//0x0054 a value between -100 and 100 for agents, Inf for items
			GW::GamePos pos1; // similar to position, maybe patrol destination? {Inf, Inf, 0} for items
			DWORD unk16;	//0x0064 always zero
			GW::GamePos pos2; // same as pos1
		};
		const DWORD Packet<P021>::STATIC_HEADER = 21;

		struct P022 : Packet<P022> {
			DWORD agent_id;
		};
		const DWORD Packet<P022>::STATIC_HEADER = 22;

		struct P023 : Packet<P023> {
			// DWORD
			// DWORD
		};
		const DWORD Packet<P023>::STATIC_HEADER = 23;

		struct P024 : Packet<P024> {
			// DWORD
			// DWORD
		};
		const DWORD Packet<P024>::STATIC_HEADER = 24;

		struct P025 : Packet<P025> {
			// DWORD
			// DWORD
		};
		const DWORD Packet<P025>::STATIC_HEADER = 25;

		struct P026 : Packet<P026> {
			// DWORD
			// Vector2f
			// BYTE
		};
		const DWORD Packet<P026>::STATIC_HEADER = 26;

		struct P027 : Packet<P027> {
			// DWORD
			// BYTE
		};
		const DWORD Packet<P027>::STATIC_HEADER = 27;

		struct P028 : Packet<P028> {
			// DWORD
			// float
		};
		const DWORD Packet<P028>::STATIC_HEADER = 28;

		struct P029 : Packet<P029> {
			// DWORD
		};
		const DWORD Packet<P029>::STATIC_HEADER = 29;

		// Movement aim
		struct P030 : Packet<P030> {
			DWORD agent_id;
			GW::GamePos pos;
			// there might be another unk field
		};
		const DWORD Packet<P030>::STATIC_HEADER = 30;

		struct P031 : Packet<P031> {
			// DWORD
			// Vector2f
			// WORD
			// WORD
			// DWORD
		};
		const DWORD Packet<P031>::STATIC_HEADER = 31;

		// Movement speed modifier
		struct P032 : Packet<P032> {
			DWORD agent_id;
			float speed;
			BYTE move_type; // ?
		};
		const DWORD Packet<P032>::STATIC_HEADER = 32;

		// Update Agent Position packet
		struct P033 : Packet<P033> {
			DWORD agent_id;
			GW::GamePos pos;
		};
		const DWORD Packet<P033>::STATIC_HEADER = 33;

		struct P034 : Packet<P034> {
			// DWORD
		};
		const DWORD Packet<P034>::STATIC_HEADER = 34;

		// Update Agent Rotation
		struct P035 : Packet<P035> {
			// DWORD
			// DWORD
			// DWORD
		};
		const DWORD Packet<P035>::STATIC_HEADER = 35;

		struct P036 : Packet<P036> {
			// DWORD
			// DWORD
		};
		const DWORD Packet<P036>::STATIC_HEADER = 36;

		struct P037 : Packet<P037> {
			// wchar_t array[32] // prefixType="int16"
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
		};
		const DWORD Packet<P037>::STATIC_HEADER = 37;

		struct P038 : Packet<P038> {
			// wchar_t array[32] // prefixType="int16"
		};
		const DWORD Packet<P038>::STATIC_HEADER = 38;

		struct P039 : Packet<P039> {
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
		};
		const DWORD Packet<P039>::STATIC_HEADER = 39;

		struct P040 : Packet<P040> {
			// wchar_t array[64] // prefixType="int16"
		};
		const DWORD Packet<P040>::STATIC_HEADER = 40;

		// creates the "ping" on an enemy when some player targets it
		struct P041 : Packet<P041> {
			DWORD Player; // who sent the message
			DWORD agent_id; // target agent
		};
		const DWORD Packet<P041>::STATIC_HEADER = 41;

		struct P042 : Packet<P042> {
			// DWORD agent_id;
			// DWORD
			// DWORD
		};
		const DWORD Packet<P042>::STATIC_HEADER = 42;

		struct P043 : Packet<P043> {
			// DWORD agent_id;
			// DWORD
		};
		const DWORD Packet<P043>::STATIC_HEADER = 43;

		struct P044 : Packet<P044> {
			// DWORD agent_id;
			// BYTE
			// BYTE
		};
		const DWORD Packet<P044>::STATIC_HEADER = 44;

		struct P045 : Packet<P045> {
			// DWORD agent_id;
			// BYTE
		};
		const DWORD Packet<P045>::STATIC_HEADER = 45;

		struct P046 : Packet<P046> {
			// DWORD agent_id;
			// BYTE
		};
		const DWORD Packet<P046>::STATIC_HEADER = 46;

		struct P047 : Packet<P047> {
			// DWORD agent_id;
			// int[48] // prefixType="int16"
		};
		const DWORD Packet<P047>::STATIC_HEADER = 47;

		struct P048 : Packet<P048> {
			// DWORD agent_id;
			// BYTE
			// BYTE
			// BYTE
		};
		const DWORD Packet<P048>::STATIC_HEADER = 48;

		struct P049 : Packet<P049> {
			// wchar_t array[8] // prefixType="int16"
			// wchar_t array[128] // prefixType="int16"
			// BYTE
			// DWORD
		};
		const DWORD Packet<P049>::STATIC_HEADER = 49;

		struct P050 : Packet<P050> {
			// DWORD agent_id;
		};
		const DWORD Packet<P050>::STATIC_HEADER = 50;

		struct P051 : Packet<P051> {
			// DWORD agent_id1;
			// DWORD agent_id2;
			// WORD
			// DWORD
			// DWORD
		};
		const DWORD Packet<P051>::STATIC_HEADER = 51;

		struct P052_RemoveExternalBond : Packet<P052_RemoveExternalBond> {
			DWORD agent_id;
			DWORD effect_id;
		};
		const DWORD Packet<P052_RemoveExternalBond>::STATIC_HEADER = 52;

		struct P053_AddExternalBond : Packet<P053_AddExternalBond> { // prety much a bond from someone else / hero bond
			DWORD caster_id;
			DWORD receiver_id;
			DWORD skill_id;
			DWORD effect_type;
			DWORD effect_id;
		};
		const DWORD Packet<P053_AddExternalBond>::STATIC_HEADER = 53;

		struct P054_AddEffect : Packet<P054_AddEffect> { // Skill applied on yourself & pcons
			DWORD agent_id;
			DWORD skill_id; // see: http://wiki.guildwars.com/wiki/Guild_Wars_Wiki:Game_integration
			DWORD effect_type;
			DWORD effect_id; // id synchronise with the server so watch out
			float duration;
		};
		const DWORD Packet<P054_AddEffect>::STATIC_HEADER = 54;

		struct P055_ReApplyEffect : Packet<P055_ReApplyEffect> {
			DWORD agent_id;
			DWORD unk; // alway 0
			DWORD effect_id;
			float duration;
		};
		const DWORD Packet<P055_ReApplyEffect>::STATIC_HEADER = 55;

		struct P056_RemoveEffect : Packet<P056_RemoveEffect> {
			DWORD agent_id;
			DWORD effect_id;
		};
		const DWORD Packet<P056_RemoveEffect>::STATIC_HEADER = 56;

		struct P057 : Packet<P057> {
			// DWORD
			// DWORD
		};
		const DWORD Packet<P057>::STATIC_HEADER = 57;

		struct P058 : Packet<P058> {
			// DWORD
			// DWORD
			// DWORD agent_id;
		};
		const DWORD Packet<P058>::STATIC_HEADER = 58;

		struct P059 : Packet<P059> {
			// DWORD
			// DWORD
			// Vector2f
			// WORD
		};
		const DWORD Packet<P059>::STATIC_HEADER = 59;

		struct P060 : Packet<P060> {
			// DWORD agent_id;
			// BYTE
		};
		const DWORD Packet<P060>::STATIC_HEADER = 60;

		// quest add compass
		struct P061 : Packet<P061> {
			DWORD quest_id; // ?
			Vector2f coordinates;
			DWORD unk1;
			DWORD unk2;
			wchar_t array1[8]; // prefixType="int16" // category?
			wchar_t array2[8]; // prefixType="int16" // name?
			wchar_t array3[8]; // prefixType="int16" // givenby?
			DWORD map_id; // ?
		};
		const DWORD Packet<P061>::STATIC_HEADER = 61;

		struct P062 : Packet<P062> {
			// DWORD
		};
		const DWORD Packet<P062>::STATIC_HEADER = 62;

		struct P063 : Packet<P063> {
			// int[64] // prefixType="int16"
		};
		const DWORD Packet<P063>::STATIC_HEADER = 63;

		struct P064 : Packet<P064> {
			// DWORD
			// wchar_t array[128] // prefixType="int16"
			// wchar_t array[128] // prefixType="int16"
		};
		const DWORD Packet<P064>::STATIC_HEADER = 64;

		struct P065 : Packet<P065> {
			// DWORD
			// Vector2f
			// WORD
			// WORD
		};
		const DWORD Packet<P065>::STATIC_HEADER = 65;

		struct P066 : Packet<P066> {
			// DWORD
			// DWORD
			// DWORD
		};
		const DWORD Packet<P066>::STATIC_HEADER = 66;

		struct P067 : Packet<P067> {
		};
		const DWORD Packet<P067>::STATIC_HEADER = 67;

		struct P068 : Packet<P068> {
			// DWORD
			// DWORD
			// wchar_t array[8] // prefixType="int16"
			// wchar_t array[8] // prefixType="int16"
			// wchar_t array[8] // prefixType="int16"
			// WORD
		};
		const DWORD Packet<P068>::STATIC_HEADER = 68;

		struct P069 : Packet<P069> {
			// DWORD
			// Vector2f
			// WORD
			// WORD
		};
		const DWORD Packet<P069>::STATIC_HEADER = 69;

		struct P070 : Packet<P070> {
			// DWORD
		};
		const DWORD Packet<P070>::STATIC_HEADER = 70;

		struct P071 : Packet<P071> {
			// DWORD
			// Vector2f
			// WORD
			// WORD
		};
		const DWORD Packet<P071>::STATIC_HEADER = 71;

		struct P072 : Packet<P072> {
			// DWORD
			// wchar_t array[128] // prefixType="int16"
		};
		const DWORD Packet<P072>::STATIC_HEADER = 72;

		struct P073 : Packet<P073> {
		};
		const DWORD Packet<P073>::STATIC_HEADER = 73;

		struct P074_NpcGeneralStats : Packet<P074_NpcGeneralStats> {
			DWORD npc_id;
			DWORD file_id;
			DWORD data1;
			DWORD scale;
			DWORD data2;
			DWORD profession_flags;
			BYTE unk1;
			BYTE unk2;
			wchar_t unk3[8];
		};
		const DWORD Packet<P074_NpcGeneralStats>::STATIC_HEADER = 74;

		struct P075 : Packet<P075> {
			DWORD npc_id;
			// int[8] // prefixType="int16"
		};
		const DWORD Packet<P075>::STATIC_HEADER = 75;

		struct P076 : Packet<P076> {
			// DWORD
			// wchar_t array[32] // prefixType="int16"
		};
		const DWORD Packet<P076>::STATIC_HEADER = 76;

		struct P077 : Packet<P077> {
			// DWORD
			// DWORD agent_id;
			// DWORD
			// BYTE
			// DWORD
			// DWORD
			// wchar_t array[32] // prefixType="int16"
		};
		const DWORD Packet<P077>::STATIC_HEADER = 77;

		struct P078 : Packet<P078> {
			// DWORD
		};
		const DWORD Packet<P078>::STATIC_HEADER = 78;

		struct P079 : Packet<P079> {
			// DWORD
		};
		const DWORD Packet<P079>::STATIC_HEADER = 79;

		struct P080 : Packet<P080> {
			// DWORD agent_id1;
			// DWORD agent_id2;
			// DWORD
		};
		const DWORD Packet<P080>::STATIC_HEADER = 80;

		// Define chat message
		struct P081 : Packet<P081> {
			wchar_t message[122]; // prefixType="int16"
		};
		const DWORD Packet<P081>::STATIC_HEADER = 81;

		enum ChatChannel {
			Alliance,	// sender: orange		message: white
			Unk1,		// sender: dark blue	message: white
			Unk2,		// sender: light blue	message: light blue
			All,		// sender: yellow		message: white
			Unk4,		// sender: light grey	message: dark grey
			Unk5,		// sender: Cyan			message: cyan
			Unk6,		// sender: white		message: white
			Warning,	// sender: -none-		message: red (in middle of screen)
			Unk8,		// sender: light grey	message: cyan
			Guild,		// sender: green		message: white
			Global,		// sender: light green	message: light green
			Group,		// sender: blue			message: white
			Trade,		// sender: light pink	message: light pink
			Advisory,	// sender: dark orange	message: dark orange
			Whisper,	// sender: blue			message: white
		};

		// Deliver chat message (no owner)
		struct P082 : Packet<P082> {
			DWORD id; // some kind of ID of the affected target
			DWORD type; // enum ChatChannel above.
		};
		const DWORD Packet<P082>::STATIC_HEADER = 82;

		struct P083 : Packet<P083> {
			// DWORD agent_id;
			// BYTE
			// wchar_t array[8] // prefixType="int16"
		};
		const DWORD Packet<P083>::STATIC_HEADER = 83;

		// Chat, identifies sender
		struct P084 : Packet<P084> {
			DWORD id; // some kind of ID
			wchar_t sender_name[32]; // full in-game name
			wchar_t sender_guild[6]; // guild tag for alliance chat, empty for guild chat
		};
		const DWORD Packet<P084>::STATIC_HEADER = 84;

		// Deliver chat message (player sender)
		struct P085 : Packet<P085> {
			DWORD id; // some kind of ID of the sender
			DWORD type; // enum ChatChannel above.
		};
		const DWORD Packet<P085>::STATIC_HEADER = 85;

		struct P086 : Packet<P086> {
			// DWORD agent_id;
			// DWORD
		};
		const DWORD Packet<P086>::STATIC_HEADER = 86;

		struct P087 : Packet<P087> {
			// DWORD agent_id1;
			// DWORD agent_id2;
		};
		const DWORD Packet<P087>::STATIC_HEADER = 87;

		struct P088 : Packet<P088> {
			// DWORD agent_id;
			// BYTE
			// BYTE
		};
		const DWORD Packet<P088>::STATIC_HEADER = 88;

		struct P089 : Packet<P089> {
			// DWORD agent_id;
			// BYTE
		};
		const DWORD Packet<P089>::STATIC_HEADER = 89;

		struct P090 : Packet<P090> {
			// DWORD agent_id;
			// Vector2f
			// WORD
		};
		const DWORD Packet<P090>::STATIC_HEADER = 90;

		struct P091 : Packet<P091> {
			// Vector2f
			// WORD
		};
		const DWORD Packet<P091>::STATIC_HEADER = 91;

		struct P092 : Packet<P092> {
			// DWORD agent_id;
			// WORD
			// DWORD
		};
		const DWORD Packet<P092>::STATIC_HEADER = 92;

		struct P093 : Packet<P093> {
			// DWORD agent_id;
		};
		const DWORD Packet<P093>::STATIC_HEADER = 93;

		struct P094 : Packet<P094> {
			// DWORD
		};
		const DWORD Packet<P094>::STATIC_HEADER = 94;

		struct P095 : Packet<P095> {
			// BYTE
			// DWORD
		};
		const DWORD Packet<P095>::STATIC_HEADER = 95;

		struct P096 : Packet<P096> {
			// DWORD
			// DWORD
			// DWORD
		};
		const DWORD Packet<P096>::STATIC_HEADER = 96;

		struct P097 : Packet<P097> {
			// DWORD agent_id;
			// DWORD
			// DWORD
		};
		const DWORD Packet<P097>::STATIC_HEADER = 97;

		struct P098 : Packet<P098> {
			// DWORD agent_id;
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
		};
		const DWORD Packet<P098>::STATIC_HEADER = 98;

		struct P099 : Packet<P099> {
			// DWORD agent_id;
			// DWORD
			// DWORD
		};
		const DWORD Packet<P099>::STATIC_HEADER = 99;

		struct P100 : Packet<P100> {
			// BYTE
		};
		const DWORD Packet<P100>::STATIC_HEADER = 100;

		struct P101 : Packet<P101> {
			// DWORD agent_id;
		};
		const DWORD Packet<P101>::STATIC_HEADER = 101;

		struct P102 : Packet<P102> {
			// WORD
			// DWORD agent_id;
			// DWORD
			// DWORD
		};
		const DWORD Packet<P102>::STATIC_HEADER = 102;

		struct P103 : Packet<P103> {
			// WORD
			// BYTE
			// BYTE
			// BYTE
			// DWORD
			// DWORD
			// BYTE
			// BYTE
		};
		const DWORD Packet<P103>::STATIC_HEADER = 103;

		struct P104 : Packet<P104> {
			// WORD
			// BYTE
			// BYTE
			// BYTE
			// DWORD
			// DWORD
			// BYTE
			// BYTE
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// wchar_t array[32] // prefixType="int16"
		};
		const DWORD Packet<P104>::STATIC_HEADER = 104;

		struct P105 : Packet<P105> {
			// DWORD agent_id;
		};
		const DWORD Packet<P105>::STATIC_HEADER = 105;

		struct P106 : Packet<P106> {
			// WORD
		};
		const DWORD Packet<P106>::STATIC_HEADER = 106;

		struct P107 : Packet<P107> {
			// WORD
		};
		const DWORD Packet<P107>::STATIC_HEADER = 107;

		struct P108 : Packet<P108> {
			// BYTE
			// <Field occurs="32" prefixType="int8" type="nested">
			// WORD
			// BYTE
			// DWORD
			// DWORD
			// BYTE
		};
		const DWORD Packet<P108>::STATIC_HEADER = 108;

		struct P109 : Packet<P109> {
			// WORD
			// DWORD
		};
		const DWORD Packet<P109>::STATIC_HEADER = 109;

		struct P110 : Packet<P110> {
			// DWORD
			// wchar_t array[8] // prefixType="int16"
			// DWORD
			// DWORD
			// DWORD
		};
		const DWORD Packet<P110>::STATIC_HEADER = 110;

		struct P111 : Packet<P111> {
			// DWORD
		};
		const DWORD Packet<P111>::STATIC_HEADER = 111;

		struct P112 : Packet<P112> {
			// DWORD
		};
		const DWORD Packet<P112>::STATIC_HEADER = 112;

		struct P113 : Packet<P113> {
			// DWORD
		};
		const DWORD Packet<P113>::STATIC_HEADER = 113;

		struct P114_DialogButton : Packet<P114_DialogButton> {
			DWORD button_style;
			wchar_t array[128]; // hstring
			DWORD dialog_id; // dialog id send when button is clicked
			DWORD skill_id; // skill / effect id if button_style is skill.
		};
		const DWORD Packet<P114_DialogButton>::STATIC_HEADER = 114;

		struct P115 : Packet<P115> {
		};
		const DWORD Packet<P115>::STATIC_HEADER = 115;

		struct P116_DialogBody : Packet<P116_DialogBody> {
			wchar_t array[122]; // hstring
		};
		const DWORD Packet<P116_DialogBody>::STATIC_HEADER = 116;

		struct P117_DialogSender : Packet<P117_DialogSender> {
			DWORD agent_id;
		};
		const DWORD Packet<P117_DialogSender>::STATIC_HEADER = 117;

		struct P118 : Packet<P118> {
			// DWORD agent_id;
			// WORD
		};
		const DWORD Packet<P118>::STATIC_HEADER = 118;

		struct P119_OpenDataWindow : Packet<P119_OpenDataWindow> {
			 DWORD agent_id;
			 DWORD windowtype;
			 DWORD val;
		};
		const DWORD Packet<P119_OpenDataWindow>::STATIC_HEADER = 119;

		struct P120 : Packet<P120> {
			// int[16] // prefixType="int16"
		};
		const DWORD Packet<P120>::STATIC_HEADER = 120;

		struct P121 : Packet<P121> {
			// BYTE
		};
		const DWORD Packet<P121>::STATIC_HEADER = 121;

		struct P122 : Packet<P122> {
			// BYTE
		};
		const DWORD Packet<P122>::STATIC_HEADER = 122;

		struct P123 : Packet<P123> {
			// BYTE
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// WORD
			// WORD
			// WORD
			// WORD
			// WORD
			// BYTE
			// BYTE
			// BYTE
			// BYTE
			// BYTE
		};
		const DWORD Packet<P123>::STATIC_HEADER = 123;

		struct P124 : Packet<P124> {
			// BYTE
		};
		const DWORD Packet<P124>::STATIC_HEADER = 124;

		struct P125 : Packet<P125> {
			// BYTE
			// Vector2f
			// Vector2f
		};
		const DWORD Packet<P125>::STATIC_HEADER = 125;

		struct P126 : Packet<P126> {
			// int[64] // prefixType="int16"
		};
		const DWORD Packet<P126>::STATIC_HEADER = 126;

		struct P127 : Packet<P127> {
			// DWORD
			// DWORD
			// DWORD
		};
		const DWORD Packet<P127>::STATIC_HEADER = 127;

		struct P128 : Packet<P128> {
			// DWORD
			// DWORD
		};
		const DWORD Packet<P128>::STATIC_HEADER = 128;

		struct P129 : Packet<P129> {
			// DWORD
			// Vector2f
			// WORD
			// BYTE
			// DWORD
			// DWORD
			// wchar_t array[8] // prefixType="int16"
		};
		const DWORD Packet<P129>::STATIC_HEADER = 129;

		struct P130 : Packet<P130> {
			// DWORD
		};
		const DWORD Packet<P130>::STATIC_HEADER = 130;

		struct P131 : Packet<P131> {
			// DWORD
			// BYTE
		};
		const DWORD Packet<P131>::STATIC_HEADER = 131;

		struct P132 : Packet<P132> {
			// DWORD
			// DWORD
		};
		const DWORD Packet<P132>::STATIC_HEADER = 132;

		// Pings and drawing in compass
		struct P133 : Packet<P133> {
			DWORD Player;    // player who sent the ping
			DWORD SessionID; // Changes for different pings/lines/curves
			DWORD NumberPts; // Number of points in the data, between 1 and 8
			struct {
				short x;	 // world coordinates divided by 10
				short y;	 // same
			} points[8];
		};
		const DWORD Packet<P133>::STATIC_HEADER = 133;

		struct P134 : Packet<P134> {
			// DWORD
			// DWORD
		};
		const DWORD Packet<P134>::STATIC_HEADER = 134;

		struct P135 : Packet<P135> {
			// DWORD agent_id;
			// DWORD
		};
		const DWORD Packet<P135>::STATIC_HEADER = 135;

		struct P136 : Packet<P136> {
			// int[32] // prefixType="int16"
			// int[32] // prefixType="int16"
			// int[32] // prefixType="int16"
			// int[32] // prefixType="int16"
			// int[32] // prefixType="int16"
		};
		const DWORD Packet<P136>::STATIC_HEADER = 136;

		struct P137 : Packet<P137> {
			// WORD
			// DWORD
		};
		const DWORD Packet<P137>::STATIC_HEADER = 137;

		struct P138 : Packet<P138> {
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
		};
		const DWORD Packet<P138>::STATIC_HEADER = 138;

		struct P139 : Packet<P139> {
			// BYTE
			// wchar_t array[128] // prefixType="int16"
		};
		const DWORD Packet<P139>::STATIC_HEADER = 139;

		struct P140 : Packet<P140> {
			// BYTE
			// DWORD
			// DWORD
			// DWORD
		};
		const DWORD Packet<P140>::STATIC_HEADER = 140;

		struct P141 : Packet<P141> {
			// WORD
			// BYTE
		};
		const DWORD Packet<P141>::STATIC_HEADER = 141;

		struct P142 : Packet<P142> {
			 DWORD agent_id;
			 DWORD scale;
		};
		const DWORD Packet<P142>::STATIC_HEADER = 142;

		struct P143 : Packet<P143> {
			// DWORD agent_id;
			// wchar_t array[32] // prefixType="int16"
		};
		const DWORD Packet<P143>::STATIC_HEADER = 143;

		struct P144 : Packet<P144> {
			// DWORD agent_id;
			// DWORD
		};
		const DWORD Packet<P144>::STATIC_HEADER = 144;

		struct P145 : Packet<P145> {
			// DWORD agent_id;
			// DWORD
			// DWORD
		};
		const DWORD Packet<P145>::STATIC_HEADER = 145;

		struct P146 : Packet<P146> {
			// DWORD agent_id;
			// wchar_t array[32] // prefixType="int16"
			// BYTE
			// wchar_t array[122] // prefixType="int16"
		};
		const DWORD Packet<P146>::STATIC_HEADER = 146;

		struct P147 : Packet<P147> {
			// DWORD
			// DWORD agent_id;
			// DWORD
		};
		const DWORD Packet<P147>::STATIC_HEADER = 147;

		struct P148 : Packet<P148> {
			// DWORD
			// DWORD agent_id;
			// DWORD agent_id;
			// DWORD
		};
		const DWORD Packet<P148>::STATIC_HEADER = 148;

		struct P149 : Packet<P149> {
			// Vector2f
			// WORD
			// DWORD agent_id;
			// WORD
			// BYTE
			// BYTE
		};
		const DWORD Packet<P149>::STATIC_HEADER = 149;

		struct P150 : Packet<P150> {
			// DWORD
			// DWORD agent_id;
			// DWORD
		};
		const DWORD Packet<P150>::STATIC_HEADER = 150;

		// damage or healing done packet, but also has other purposes.
		// to be investigated further.
		// all types have their value in the float field 'value'.
		// in all types the value is in percentage, unless otherwise specified.
		// the value can be negative (e.g. damage, sacrifice) 
		// or positive (e.g. heal, lifesteal).
		namespace P151_Type {
			const DWORD damage = 16; // non-armor-ignoring attack, spells
			const DWORD critical = 17; // critical hit on autoattack
			const DWORD energygain = 52; // for example from Critical Strikes or energy tap
			const DWORD armorignoring = 55; // all armor ignoring damage and heals
			const DWORD casttime = 61; // non-standard cast time, value in seconds
		}
		struct P151 : Packet<P151> {
			DWORD type;			// type as specified above in P151_Type
			DWORD target_id;	// agent id of who is affected by the change
			DWORD cause_id;		// agent id of who caused the change
			float value;		// value, often in percentage (e.g. %hp)
		};
		const DWORD Packet<P151>::STATIC_HEADER = 151;

		struct P152 : Packet<P152> {
			// DWORD agent_id;
			// Vector2f
			// WORD
			// DWORD
			// DWORD
			// DWORD
			// BYTE
		};
		const DWORD Packet<P152>::STATIC_HEADER = 152;

		struct P153 : Packet<P153> {
			// DWORD agent_id;
			// wchar_t array[122] // prefixType="int16"
		};
		const DWORD Packet<P153>::STATIC_HEADER = 153;

		struct P154 : Packet<P154> {
			// DWORD agent_id;
			// BYTE
			// BYTE
		};
		const DWORD Packet<P154>::STATIC_HEADER = 154;

		struct P155 : Packet<P155> {
			// DWORD agent_id;
			// DWORD
			// DWORD
		};
		const DWORD Packet<P155>::STATIC_HEADER = 155;

		struct P156 : Packet<P156> {
			// Vector2f
			// WORD
			// DWORD
		};
		const DWORD Packet<P156>::STATIC_HEADER = 156;

		struct P157 : Packet<P157> {
			// DWORD agent_id;
			// DWORD
		};
		const DWORD Packet<P157>::STATIC_HEADER = 157;

		struct P158 : Packet<P158> {
			// DWORD agent_id;
			// DWORD
			// DWORD
		};
		const DWORD Packet<P158>::STATIC_HEADER = 158;

		struct P159 : Packet<P159> {
		};
		const DWORD Packet<P159>::STATIC_HEADER = 159;

		struct P160 : Packet<P160> {
			// DWORD agent_id;
			// BYTE
		};
		const DWORD Packet<P160>::STATIC_HEADER = 160;

		struct P161 : Packet<P161> {
			// DWORD
			// BYTE
		};
		const DWORD Packet<P161>::STATIC_HEADER = 161;

		// agent change model
		struct P162 : Packet<P162> {
			 DWORD agent_id;
			 DWORD model_id;
		};
		const DWORD Packet<P162>::STATIC_HEADER = 162;

		struct P163 : Packet<P163> {
			// wchar_t array[32] // prefixType="int16"
		};
		const DWORD Packet<P163>::STATIC_HEADER = 163;

		struct P164 : Packet<P164> {
			// WORD
			// BYTE
		};
		const DWORD Packet<P164>::STATIC_HEADER = 164;

		struct P165 : Packet<P165> {
			// WORD
			// WORD
		};
		const DWORD Packet<P165>::STATIC_HEADER = 165;

		struct P166 : Packet<P166> {
			// DWORD agent_id;
			// DWORD agent_id;
			// wchar_t array[32] // prefixType="int16"
			// DWORD
			// DWORD
			// DWORD
		};
		const DWORD Packet<P166>::STATIC_HEADER = 166;

		struct P167 : Packet<P167> {
			// DWORD agent_id;
		};
		const DWORD Packet<P167>::STATIC_HEADER = 167;

		struct P168 : Packet<P168> {
			// DWORD agent_id;
			// wchar_t array[32] // prefixType="int16"
		};
		const DWORD Packet<P168>::STATIC_HEADER = 168;

		struct P169 : Packet<P169> {
			// <Field occurs="256" prefixType="int16" type="int8"/>
		};
		const DWORD Packet<P169>::STATIC_HEADER = 169;
		
		struct P170 : Packet<P170> {
			// DWORD agent_id;
			// DWORD
		};
		const DWORD Packet<P170>::STATIC_HEADER = 170;

		struct P171 : Packet<P171> {
			// DWORD agent_id;
			// BYTE
			// BYTE
			// BYTE
		};
		const DWORD Packet<P171>::STATIC_HEADER = 171;

		struct P172 : Packet<P172> {
		};
		const DWORD Packet<P172>::STATIC_HEADER = 172;

		struct P173 : Packet<P173> {
			// wchar_t array[128] // prefixType="int16"
			// DWORD
		};
		const DWORD Packet<P173>::STATIC_HEADER = 173;

		struct P174 : Packet<P174> {
		};
		const DWORD Packet<P174>::STATIC_HEADER = 174;

		struct P175 : Packet<P175> {
			// DWORD
			// DWORD
			// wchar_t array[128] // prefixType="int16"
		};
		const DWORD Packet<P175>::STATIC_HEADER = 175;

		struct P176 : Packet<P176> {
			// DWORD
		};
		const DWORD Packet<P176>::STATIC_HEADER = 176;

		struct P177 : Packet<P177> {
			// DWORD
			// wchar_t array[128] // prefixType="int16"
		};
		const DWORD Packet<P177>::STATIC_HEADER = 177;

		struct P178 : Packet<P178> {
			// DWORD agent_id;
		};
		const DWORD Packet<P178>::STATIC_HEADER = 178;

		struct P179 : Packet<P179> {
			// int[64] // prefixType="int16"
		};
		const DWORD Packet<P179>::STATIC_HEADER = 179;

		struct P180 : Packet<P180> {
			// DWORD
			// DWORD
			// DWORD
			// wchar_t array[128] // prefixType="int16"
			// wchar_t array[128] // prefixType="int16"
		};
		const DWORD Packet<P180>::STATIC_HEADER = 180;

		struct P181 : Packet<P181> {
			// DWORD
			// DWORD
		};
		const DWORD Packet<P181>::STATIC_HEADER = 181;

		struct P182 : Packet<P182> {
			// DWORD
			// DWORD
		};
		const DWORD Packet<P182>::STATIC_HEADER = 182;

		struct P183 : Packet<P183> {
			// BYTE
			// DWORD
		};
		const DWORD Packet<P183>::STATIC_HEADER = 183;

		struct P184 : Packet<P184> {
			// DWORD agent_id;
		};
		const DWORD Packet<P184>::STATIC_HEADER = 184;

		struct P185 : Packet<P185> {
			// BYTE
			// wchar_t array[122] // prefixType="int16"
		};
		const DWORD Packet<P185>::STATIC_HEADER = 185;

		struct P186 : Packet<P186> {
			// BYTE
			// DWORD
		};
		const DWORD Packet<P186>::STATIC_HEADER = 186;

		struct P187 : Packet<P187> {
			// BYTE
			// DWORD
			// int[16] // prefixType="int16"
		};
		const DWORD Packet<P187>::STATIC_HEADER = 187;

		struct P188 : Packet<P188> {
			// BYTE
			// DWORD
			// DWORD
		};
		const DWORD Packet<P188>::STATIC_HEADER = 188;

		struct P189 : Packet<P189> {
			// BYTE
			// BYTE
		};
		const DWORD Packet<P189>::STATIC_HEADER = 189;

		struct P190 : Packet<P190> {
			// BYTE
			// DWORD
		};
		const DWORD Packet<P190>::STATIC_HEADER = 190;

		struct P191 : Packet<P191> {
			// BYTE
		};
		const DWORD Packet<P191>::STATIC_HEADER = 191;

		struct P192 : Packet<P192> {
			// BYTE
		};
		const DWORD Packet<P192>::STATIC_HEADER = 192;

		struct P193 : Packet<P193> {
			// BYTE
			// DWORD
			// BYTE
			// BYTE
		};
		const DWORD Packet<P193>::STATIC_HEADER = 193;

		struct P194 : Packet<P194> {
			// DWORD
			// DWORD
		};
		const DWORD Packet<P194>::STATIC_HEADER = 194;

		struct P195 : Packet<P195> {
			// DWORD agent_id;
			// DWORD
		};
		const DWORD Packet<P195>::STATIC_HEADER = 195;

		struct P196 : Packet<P196> {
			// DWORD agent_id;
		};
		const DWORD Packet<P196>::STATIC_HEADER = 196;

		struct P197 : Packet<P197> {
			// DWORD agent_id;
			// WORD
			// DWORD
			// DWORD
		};
		const DWORD Packet<P197>::STATIC_HEADER = 197;

		struct P198 : Packet<P198> {
			// DWORD agent_id;
			// WORD
			// DWORD
		};
		const DWORD Packet<P198>::STATIC_HEADER = 198;

		struct P199 : Packet<P199> {
			// int[128] // prefixType="int16"
		};
		const DWORD Packet<P199>::STATIC_HEADER = 199;

		struct P200 : Packet<P200> {
		};
		const DWORD Packet<P200>::STATIC_HEADER = 200;

		struct P201 : Packet<P201> {
		};
		const DWORD Packet<P201>::STATIC_HEADER = 201;

		struct P202 : Packet<P202> {
			// WORD
			// WORD
			// DWORD
		};
		const DWORD Packet<P202>::STATIC_HEADER = 202;

		struct P203 : Packet<P203> {
			// int[8] // prefixType="int16"
		};
		const DWORD Packet<P203>::STATIC_HEADER = 203;

		struct P204 : Packet<P204> {
			// int[4] // prefixType="int16"
		};
		const DWORD Packet<P204>::STATIC_HEADER = 204;

		struct P205 : Packet<P205> {
			// DWORD agent_id;
			// BYTE
			// WORD
			// DWORD
		};
		const DWORD Packet<P205>::STATIC_HEADER = 205;

		struct P206 : Packet<P206> {
			// DWORD agent_id;
			// int[8] // prefixType="int16"
			// int[8] // prefixType="int16"
			// BYTE
		};
		const DWORD Packet<P206>::STATIC_HEADER = 206;

		struct P207 : Packet<P207> {
			// int[128] // prefixType="int16"
		};
		const DWORD Packet<P207>::STATIC_HEADER = 207;

		struct P208 : Packet<P208> {
			// WORD
			// DWORD
		};
		const DWORD Packet<P208>::STATIC_HEADER = 208;

		struct P209 : Packet<P209> {
			// WORD
			// DWORD
		};
		const DWORD Packet<P209>::STATIC_HEADER = 209;

		struct P210 : Packet<P210> {
			// WORD
		};
		const DWORD Packet<P210>::STATIC_HEADER = 210;

		struct P211 : Packet<P211> {
			// DWORD agent_id;
			// DWORD
		};
		const DWORD Packet<P211>::STATIC_HEADER = 211;

		struct P212 : Packet<P212> {
			// BYTE
			// int[16] // prefixType="int16"
		};
		const DWORD Packet<P212>::STATIC_HEADER = 212;

		struct P213 : Packet<P213> {
			// BYTE
		};
		const DWORD Packet<P213>::STATIC_HEADER = 213;

		struct P214 : Packet<P214> {
			// DWORD agent_id;
			// WORD
			// DWORD
		};
		const DWORD Packet<P214>::STATIC_HEADER = 214;

		struct P215 : Packet<P215> {
			// DWORD agent_id;
			// WORD
			// DWORD
		};
		const DWORD Packet<P215>::STATIC_HEADER = 215;

		struct P216 : Packet<P216> {
			// DWORD agent_id;
			// WORD
			// DWORD
		};
		const DWORD Packet<P216>::STATIC_HEADER = 216;

		struct P217 : Packet<P217> {
			// DWORD agent_id;
			// WORD
			// DWORD
			// DWORD
		};
		const DWORD Packet<P217>::STATIC_HEADER = 217;

		struct P218 : Packet<P218> {
			// DWORD agent_id;
			// WORD
			// DWORD
		};
		const DWORD Packet<P218>::STATIC_HEADER = 218;

		struct P219 : Packet<P219> {
			// DWORD agent_id;
			// WORD
			// DWORD
		};
		const DWORD Packet<P219>::STATIC_HEADER = 219;

		struct P220 : Packet<P220> {
			// DWORD agent_id;
			// WORD
			// DWORD
			// DWORD
			// DWORD
		};
		const DWORD Packet<P220>::STATIC_HEADER = 220;

		struct P221 : Packet<P221> {
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
		};
		const DWORD Packet<P221>::STATIC_HEADER = 221;

		struct P222 : Packet<P222> {
			// DWORD
		};
		const DWORD Packet<P222>::STATIC_HEADER = 222;

		struct P223 : Packet<P223> {
			// DWORD
		};
		const DWORD Packet<P223>::STATIC_HEADER = 223;

		struct P224 : Packet<P224> {
			// DWORD
		};
		const DWORD Packet<P224>::STATIC_HEADER = 224;

		struct P225 : Packet<P225> {
			// DWORD
		};
		const DWORD Packet<P225>::STATIC_HEADER = 225;

		struct P226 : Packet<P226> {
			// DWORD
			// DWORD
		};
		const DWORD Packet<P226>::STATIC_HEADER = 226;

		struct P227 : Packet<P227> {
			// DWORD
			// DWORD
		};
		const DWORD Packet<P227>::STATIC_HEADER = 227;

		struct P228 : Packet<P228> {
			// DWORD agent_id;
			// DWORD
		};
		const DWORD Packet<P228>::STATIC_HEADER = 228;

		struct P229 : Packet<P229> {
			// DWORD agent_id;
			// DWORD
		};
		const DWORD Packet<P229>::STATIC_HEADER = 229;

		struct P230 : Packet<P230> {
			// DWORD
		};
		const DWORD Packet<P230>::STATIC_HEADER = 230;

		struct P231 : Packet<P231> {
			// DWORD
			// DWORD
			// DWORD
			// wchar_t array[8] // prefixType="int16"
		};
		const DWORD Packet<P231>::STATIC_HEADER = 231;

		struct P232 : Packet<P232> {
			// WORD
			// DWORD
		};
		const DWORD Packet<P232>::STATIC_HEADER = 232;

		struct P233 : Packet<P233> {
			// DWORD
			// DWORD
		};
		const DWORD Packet<P233>::STATIC_HEADER = 233;

		struct P234 : Packet<P234> {
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// wchar_t array[8] // prefixType="int16"
			// wchar_t array[8] // prefixType="int16"
		};
		const DWORD Packet<P234>::STATIC_HEADER = 234;

		struct P235 : Packet<P235> {
			// DWORD
			// DWORD
		};
		const DWORD Packet<P235>::STATIC_HEADER = 235;

		struct P236 : Packet<P236> {
			// DWORD agent_id;
		};
		const DWORD Packet<P236>::STATIC_HEADER = 236;

		struct P237 : Packet<P237> {
			// int[16] // prefixType="int16"
		};
		const DWORD Packet<P237>::STATIC_HEADER = 237;

		struct P238 : Packet<P238> {
			// int[32] // prefixType="int16"
		};
		const DWORD Packet<P238>::STATIC_HEADER = 238;

		struct P239 : Packet<P239> {
			// WORD
			// DWORD
			// DWORD
		};
		const DWORD Packet<P239>::STATIC_HEADER = 239;

		struct P240 : Packet<P240> {
			// DWORD
			// DWORD
		};
		const DWORD Packet<P240>::STATIC_HEADER = 240;

		struct P241 : Packet<P241> {
			// wchar_t array[32] // prefixType="int16"
			// BYTE
		};
		const DWORD Packet<P241>::STATIC_HEADER = 241;

		struct P242 : Packet<P242> {
		};
		const DWORD Packet<P242>::STATIC_HEADER = 242;

		struct P243 : Packet<P243> {
		};
		const DWORD Packet<P243>::STATIC_HEADER = 243;

		struct P244 : Packet<P244> {
		};
		const DWORD Packet<P244>::STATIC_HEADER = 244;

		struct P245 : Packet<P245> {
			// BYTE
			// BYTE
		};
		const DWORD Packet<P245>::STATIC_HEADER = 245;

		struct P246 : Packet<P246> {
			// BYTE
		};
		const DWORD Packet<P246>::STATIC_HEADER = 246;

		struct P247 : Packet<P247> {
			// BYTE
		};
		const DWORD Packet<P247>::STATIC_HEADER = 247;

		struct P248 : Packet<P248> {
			// DWORD
			// WORD
			// wchar_t array[80] // prefixType="int16"
		};
		const DWORD Packet<P248>::STATIC_HEADER = 248;

		struct P249 : Packet<P249> {
		};
		const DWORD Packet<P249>::STATIC_HEADER = 249;

		struct P250 : Packet<P250> {
			// <Field occurs="1024" prefixType="int16" type="int8"/>
		};
		const DWORD Packet<P250>::STATIC_HEADER = 250;

		struct P251 : Packet<P251> {
		};
		const DWORD Packet<P251>::STATIC_HEADER = 251;

		struct P252 : Packet<P252> {
			// DWORD
		};
		const DWORD Packet<P252>::STATIC_HEADER = 252;

		struct P253 : Packet<P253> {
			// DWORD agent_id;
			// DWORD
			// DWORD
			// BYTE
			// wchar_t array[32] // prefixType="int16"
		};
		const DWORD Packet<P253>::STATIC_HEADER = 253;

		struct P254 : Packet<P254> {
			// DWORD agent_id;
		};
		const DWORD Packet<P254>::STATIC_HEADER = 254;

		struct P255 : Packet<P255> {
			// DWORD
			// wchar_t array[122] // prefixType="int16"
		};
		const DWORD Packet<P255>::STATIC_HEADER = 255;

		struct P256 : Packet<P256> {
			// BYTE
			// wchar_t array[122] // prefixType="int16"
			// DWORD
			// WORD
		};
		const DWORD Packet<P256>::STATIC_HEADER = 256;

		struct P257 : Packet<P257> {
			// wchar_t array[122] // prefixType="int16"
		};
		const DWORD Packet<P257>::STATIC_HEADER = 257;

		struct P258 : Packet<P258> {
			// DWORD
		};
		const DWORD Packet<P258>::STATIC_HEADER = 258;

		struct P259 : Packet<P259> {
			// DWORD
		};
		const DWORD Packet<P259>::STATIC_HEADER = 259;

		// manipulate map object
		struct P260 : Packet<P260> {
			DWORD object_id;
			DWORD unk1;
			DWORD unk2;
		};
		const DWORD Packet<P260>::STATIC_HEADER = 260;

		struct P261 : Packet<P261> {
			// WORD
			// BYTE
		};
		const DWORD Packet<P261>::STATIC_HEADER = 261;

		struct P262 : Packet<P262> {
			// WORD
			// DWORD
		};
		const DWORD Packet<P262>::STATIC_HEADER = 262;

		struct P263 : Packet<P263> {
			DWORD object_id;
			DWORD unk1;
			DWORD unk2;
		};
		const DWORD Packet<P263>::STATIC_HEADER = 263;

		struct P264 : Packet<P264> {
			// DWORD
			// BYTE
		};
		const DWORD Packet<P264>::STATIC_HEADER = 264;

		struct P265 : Packet<P265> {
			// DWORD
			// wchar_t array[32] // prefixType="int16"
		};
		const DWORD Packet<P265>::STATIC_HEADER = 265;

		struct P266 : Packet<P266> {
			// DWORD
			// DWORD
		};
		const DWORD Packet<P266>::STATIC_HEADER = 266;

		struct P267 : Packet<P267> {
			// DWORD
			// DWORD
		};
		const DWORD Packet<P267>::STATIC_HEADER = 267;

		struct P268 : Packet<P268> {
			// WORD
			// <Field occurs="16" prefixType="int16" static="true" type="int8"/>
			// BYTE
			// DWORD
		};
		const DWORD Packet<P268>::STATIC_HEADER = 268;

		struct P269 : Packet<P269> {
			// <Field occurs="16" prefixType="int16" static="true" type="int8"/>
		};
		const DWORD Packet<P269>::STATIC_HEADER = 269;

		struct P270 : Packet<P270> {
			// WORD
			// <Field occurs="16" prefixType="int16" static="true" type="int8"/>
			// BYTE
		};
		const DWORD Packet<P270>::STATIC_HEADER = 270;

		struct P271 : Packet<P271> {
		};
		const DWORD Packet<P271>::STATIC_HEADER = 271;

		struct P272 : Packet<P272> {
			// WORD
			// BYTE
			// BYTE
			// DWORD
			// wchar_t array[32] // prefixType="int16"
			// wchar_t array[6] // prefixType="int16"
			// BYTE
			// BYTE
			// BYTE
			// BYTE
			// BYTE
			// WORD
			// BYTE
		};
		const DWORD Packet<P272>::STATIC_HEADER = 272;

		struct P273 : Packet<P273> {
			// DWORD
			// BYTE
		};
		const DWORD Packet<P273>::STATIC_HEADER = 273;

		struct P274 : Packet<P274> {
		};
		const DWORD Packet<P274>::STATIC_HEADER = 274;

		struct P275 : Packet<P275> {
			// BYTE
		};
		const DWORD Packet<P275>::STATIC_HEADER = 275;

		struct P276 : Packet<P276> {
			// WORD
			// DWORD
		};
		const DWORD Packet<P276>::STATIC_HEADER = 276;

		struct P277 : Packet<P277> {
			// DWORD
			// wchar_t array[64] // prefixType="int16"
		};
		const DWORD Packet<P277>::STATIC_HEADER = 277;

		struct P278 : Packet<P278> {
			// WORD
			// <Field occurs="16" prefixType="int16" static="true" type="int8"/>
			// wchar_t array[32] // prefixType="int16"
			// wchar_t array[6] // prefixType="int16"
			// BYTE
			// BYTE
			// BYTE
			// BYTE
			// BYTE
			// BYTE
			// WORD
			// BYTE
			// BYTE
			// DWORD
			// BYTE
		};
		const DWORD Packet<P278>::STATIC_HEADER = 278;

		struct P279 : Packet<P279> {
			// WORD
			// <Field occurs="16" prefixType="int16" static="true" type="int8"/>
			// wchar_t array[32] // prefixType="int16"
			// wchar_t array[6] // prefixType="int16"
			// BYTE
			// BYTE
			// BYTE
			// BYTE
			// BYTE
			// BYTE
			// BYTE
			// WORD
			// BYTE
			// BYTE
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// BYTE
		};
		const DWORD Packet<P279>::STATIC_HEADER = 279;

		struct P280 : Packet<P280> {
			// WORD
			// BYTE
			// DWORD
		};
		const DWORD Packet<P280>::STATIC_HEADER = 280;

		struct P281 : Packet<P281> {
			// <Field occurs="16" prefixType="int16" static="true" type="int8"/>
			// wchar_t array[32] // prefixType="int16"
			// wchar_t array[6] // prefixType="int16"
			// wchar_t array[20] // prefixType="int16"
			// DWORD
			// DWORD
			// BYTE
		};
		const DWORD Packet<P281>::STATIC_HEADER = 281;

		struct P282 : Packet<P282> {
			// <Field occurs="16" prefixType="int16" static="true" type="int8"/>
			// BYTE
		};
		const DWORD Packet<P282>::STATIC_HEADER = 282;

		struct P283 : Packet<P283> {
			// WORD
			// DWORD
		};
		const DWORD Packet<P283>::STATIC_HEADER = 283;

		struct P284 : Packet<P284> {
			// WORD
			// BYTE
		};
		const DWORD Packet<P284>::STATIC_HEADER = 284;

		struct P285 : Packet<P285> {
			// wchar_t array[20] // prefixType="int16"
			// wchar_t array[20] // prefixType="int16"
			// wchar_t array[20] // prefixType="int16"
			// wchar_t array[64] // prefixType="int16"
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// BYTE
		};
		const DWORD Packet<P285>::STATIC_HEADER = 285;

		struct P286 : Packet<P286> {
			// wchar_t array[20] // prefixType="int16"
		};
		const DWORD Packet<P286>::STATIC_HEADER = 286;

		struct P287 : Packet<P287> {
			// wchar_t array[20] // prefixType="int16"
		};
		const DWORD Packet<P287>::STATIC_HEADER = 287;

		struct P288 : Packet<P288> {
			// wchar_t array[20] // prefixType="int16"
		};
		const DWORD Packet<P288>::STATIC_HEADER = 288;

		struct P289 : Packet<P289> {
			// DWORD
			// wchar_t array[64] // prefixType="int16"
		};
		const DWORD Packet<P289>::STATIC_HEADER = 289;

		struct P290 : Packet<P290> {
			// DWORD
			// DWORD
		};
		const DWORD Packet<P290>::STATIC_HEADER = 290;

		struct P291 : Packet<P291> {
			// BYTE
		};
		const DWORD Packet<P291>::STATIC_HEADER = 291;

		struct P292 : Packet<P292> {
			// WORD
		};
		const DWORD Packet<P292>::STATIC_HEADER = 292;

		struct P293 : Packet<P293> {
			// wchar_t array[256] // prefixType="int16"
			// wchar_t array[20] // prefixType="int16"
		};
		const DWORD Packet<P293>::STATIC_HEADER = 293;

		struct P294 : Packet<P294> {
			// WORD
			// DWORD
		};
		const DWORD Packet<P294>::STATIC_HEADER = 294;

		struct P295 : Packet<P295> {
		};
		const DWORD Packet<P295>::STATIC_HEADER = 295;

		struct P296 : Packet<P296> {
			// DWORD
			// BYTE
		};
		const DWORD Packet<P296>::STATIC_HEADER = 296;

		struct P297 : Packet<P297> {
			// DWORD
		};
		const DWORD Packet<P297>::STATIC_HEADER = 297;

		struct P298 : Packet<P298> {
		};
		const DWORD Packet<P298>::STATIC_HEADER = 298;

		struct P299 : Packet<P299> {
			// DWORD
			// WORD
			// DWORD
		};
		const DWORD Packet<P299>::STATIC_HEADER = 299;

		struct P300 : Packet<P300> {
			// DWORD
		};
		const DWORD Packet<P300>::STATIC_HEADER = 300;

		struct P301 : Packet<P301> {
			// DWORD
			// DWORD
		};
		const DWORD Packet<P301>::STATIC_HEADER = 301;

		struct P302 : Packet<P302> {
			// DWORD
			// BYTE
			// wchar_t array[8] // prefixType="int16"
			// wchar_t array[8] // prefixType="int16"
			// wchar_t array[8] // prefixType="int16"
			// wchar_t array[8] // prefixType="int16"
		};
		const DWORD Packet<P302>::STATIC_HEADER = 302;

		struct P303 : Packet<P303> {
			DWORD itemid;
			DWORD quantity;
		};
		const DWORD Packet<P303>::STATIC_HEADER = 303;

		struct P304 : Packet<P304> {
			// DWORD
			// wchar_t array[32] // prefixType="int16"
		};
		const DWORD Packet<P304>::STATIC_HEADER = 304;

		struct P305 : Packet<P305> {
			// DWORD
			// DWORD
		};
		const DWORD Packet<P305>::STATIC_HEADER = 305;

		struct P306 : Packet<P306> {
			// BYTE
			// DWORD
		};
		const DWORD Packet<P306>::STATIC_HEADER = 306;

		struct P307 : Packet<P307> {
			// DWORD
			// WORD
			// BYTE
		};
		const DWORD Packet<P307>::STATIC_HEADER = 307;

		struct P308 : Packet<P308> {
			// WORD
			// DWORD
			// WORD
			// BYTE
		};
		const DWORD Packet<P308>::STATIC_HEADER = 308;

		struct P309 : Packet<P309> {
			// WORD
			// BYTE
			// BYTE
			// WORD
			// BYTE
			// DWORD
		};
		const DWORD Packet<P309>::STATIC_HEADER = 309;

		struct P310 : Packet<P310> {
			// WORD
			// DWORD
		};
		const DWORD Packet<P310>::STATIC_HEADER = 310;

		struct P311 : Packet<P311> {
			// WORD
			// DWORD
		};
		const DWORD Packet<P311>::STATIC_HEADER = 311;

		struct P312 : Packet<P312> {
			// WORD
		};
		const DWORD Packet<P312>::STATIC_HEADER = 312;

		struct P313 : Packet<P313> {
			// WORD
			// WORD
		};
		const DWORD Packet<P313>::STATIC_HEADER = 313;

		struct P314 : Packet<P314> {
			// WORD
			// BYTE
		};
		const DWORD Packet<P314>::STATIC_HEADER = 314;

		struct P315 : Packet<P315> {
			// WORD
		};
		const DWORD Packet<P315>::STATIC_HEADER = 315;

		struct P316 : Packet<P316> {
			// WORD
			// BYTE
			// WORD
			// BYTE
			// DWORD
		};
		const DWORD Packet<P316>::STATIC_HEADER = 316;

		struct P317 : Packet<P317> {
			// WORD
			// BYTE
			// DWORD
			// DWORD
		};
		const DWORD Packet<P317>::STATIC_HEADER = 317;

		struct P318 : Packet<P318> {
			// WORD
			// BYTE
		};
		const DWORD Packet<P318>::STATIC_HEADER = 318;

		struct P319 : Packet<P319> {
			// WORD
			// WORD
			// BYTE
		};
		const DWORD Packet<P319>::STATIC_HEADER = 319;

		struct P320 : Packet<P320> {
		};
		const DWORD Packet<P320>::STATIC_HEADER = 320;

		struct P321 : Packet<P321> {
			// WORD
			// DWORD
			// WORD
			// BYTE
		};
		const DWORD Packet<P321>::STATIC_HEADER = 321;

		struct P322 : Packet<P322> {
			// WORD
			// BYTE
			// BYTE
		};
		const DWORD Packet<P322>::STATIC_HEADER = 322;

		struct P323 : Packet<P323> {
			// WORD
			// DWORD
		};
		const DWORD Packet<P323>::STATIC_HEADER = 323;

		struct P324 : Packet<P324> {
			// WORD
			// WORD
		};
		const DWORD Packet<P324>::STATIC_HEADER = 324;

		struct P325 : Packet<P325> {
			// WORD
			// DWORD
		};
		const DWORD Packet<P325>::STATIC_HEADER = 325;

		struct P326 : Packet<P326> {
			// WORD
			// DWORD
		};
		const DWORD Packet<P326>::STATIC_HEADER = 326;

		struct P327 : Packet<P327> {
			// WORD
			// DWORD
		};
		const DWORD Packet<P327>::STATIC_HEADER = 327;

		struct P328 : Packet<P328> {
			// WORD
			// DWORD
			// DWORD
		};
		const DWORD Packet<P328>::STATIC_HEADER = 328;

		struct P329 : Packet<P329> {
			// WORD
			// DWORD
			// WORD
			// BYTE
		};
		const DWORD Packet<P329>::STATIC_HEADER = 329;

		struct P330 : Packet<P330> {
			// DWORD
			// BYTE
			// BYTE
		};
		const DWORD Packet<P330>::STATIC_HEADER = 330;

		struct P331 : Packet<P331> {
			// DWORD
			// DWORD
		};
		const DWORD Packet<P331>::STATIC_HEADER = 331;

		struct P332 : Packet<P332> {
			// DWORD
		};
		const DWORD Packet<P332>::STATIC_HEADER = 332;

		struct P333 : Packet<P333> {
			// DWORD
			// DWORD
			// DWORD
			// DWORD
		};
		const DWORD Packet<P333>::STATIC_HEADER = 333;

		struct P334 : Packet<P334> {
			// BYTE
			// BYTE
		};
		const DWORD Packet<P334>::STATIC_HEADER = 334;

		struct P335 : Packet<P335> {
			// DWORD
			// WORD
		};
		const DWORD Packet<P335>::STATIC_HEADER = 335;

		struct P336 : Packet<P336> {
			// DWORD
			// BYTE
		};
		const DWORD Packet<P336>::STATIC_HEADER = 336;

		struct P337 : Packet<P337> {
			// DWORD
			// DWORD
		};
		const DWORD Packet<P337>::STATIC_HEADER = 337;

		struct P338 : Packet<P338> {
			// WORD
			// <Field occurs="7" prefixType="int8" type="nested">
			// BYTE
			// WORD
			// BYTE
			// WORD
			// WORD
			// WORD
			// WORD
		};
		const DWORD Packet<P338>::STATIC_HEADER = 338;

		struct P339 : Packet<P339> {
			// DWORD
			// DWORD
		};
		const DWORD Packet<P339>::STATIC_HEADER = 339;

		struct P340 : Packet<P340> {
			// DWORD
			// DWORD
			// BYTE
			// BYTE
			// WORD
			// WORD
			// BYTE
			// DWORD
			// DWORD
		};
		const DWORD Packet<P340>::STATIC_HEADER = 340;

		struct P341 : Packet<P341> {
			// DWORD
			// DWORD
			// BYTE
			// BYTE
			// WORD
			// WORD
			// BYTE
			// DWORD
			// DWORD
		};
		const DWORD Packet<P341>::STATIC_HEADER = 341;

		struct P342 : Packet<P342> {
			// DWORD
			// DWORD
			// DWORD
			// wchar_t array[64] // prefixType="int16"
			// <Field occurs="64" prefixType="int8" type="nested">
			// DWORD
		};
		const DWORD Packet<P342>::STATIC_HEADER = 342;

		// About P343 and P344: They look exactly the same. 
		// P343 is used first, for a while, then P344 is used. 
		enum ItemType : DWORD {
			// note: not all are tested but all I tested were correct
			Salvage = 0, LeadHand, Axe, Bag, Feet, Bow, Bundle, Chest, Rune, Consumable,
			Dye, Material, Focus, Arms, Sigil, Hammer, Head, SalvageItem, Key, Legs,
			Coins, QuestItem, Wand, Shield, Staff, Sword, Kit, Trophy, Scroll, Daggers,
			Present, Minipet, Scythe, Spear, Handbook, CostumeBody, CostumeHead,
		};
		namespace ItemFlags {
			// note: untested
			// all flags from 0x1 to 0x8000000. Most are to be discovered.
			const DWORD RarityUnique = 0x10; // green item
			const DWORD CannotBeDropped = 0x100;
			const DWORD RarityRare = 0x20000; // gold item
			const DWORD TwoHanded = 0x200000;
			const DWORD RarityUncommon = 0x400000; // purple item
			const DWORD Inscribable = 0x8000000;
			const DWORD Dyeable = 0x20000000;
			const DWORD Dedicated = 0x40000000;
			const DWORD Pvp = 0x1000000;
		}
		struct P343 : Packet<P343> {
			DWORD local_id; // this will be referenced by P021 just after
			DWORD file_id; // id of the 3d model in the dat ?
			ItemType item_type; // see above
			DWORD unk1;
			DWORD dye_color; // untested
			DWORD material; // untested
			DWORD unk2; 
			DWORD item_flags; // see above
			DWORD price; // 0 if cannot be sold
			DWORD item_id;
			DWORD quantity;
			// then there should be the item name.. I think.
			// wchar_t array[64] // prefixType="int16"
			// <Field occurs="64" prefixType="int8" type="nested">
			// DWORD
		};
		const DWORD Packet<P343>::STATIC_HEADER = 343;

		struct P344 : Packet<P344> {
			DWORD local_id; // this will be referenced by P021 just after
			DWORD file_id; // id of the 3d model in the dat ?
			ItemType item_type; // see above
			DWORD unk1;
			DWORD dye_color; // untested
			DWORD material; // untested
			DWORD unk2;
			DWORD item_flags; // see above
			DWORD price; // 0 if cannot be sold
			DWORD item_id;
			DWORD quantity;
			// then there should be the item name.. I think.
			// wchar_t array[64] // prefixType="int16"
			// <Field occurs="64" prefixType="int8" type="nested">
			// DWORD
		};
		const DWORD Packet<P344>::STATIC_HEADER = 344;

		struct P345 : Packet<P345> {
			// WORD
			// DWORD
			// int[3] // prefixType="int16"
		};
		const DWORD Packet<P345>::STATIC_HEADER = 345;

		struct P346 : Packet<P346> {
		};
		const DWORD Packet<P346>::STATIC_HEADER = 346;

		struct P347 : Packet<P347> {
		};
		const DWORD Packet<P347>::STATIC_HEADER = 347;

		struct P348 : Packet<P348> {
		};
		const DWORD Packet<P348>::STATIC_HEADER = 348;

		struct P349 : Packet<P349> {
			// BYTE
		};
		const DWORD Packet<P349>::STATIC_HEADER = 349;

		struct P350 : Packet<P350> {
			// WORD
			// WORD
		};
		const DWORD Packet<P350>::STATIC_HEADER = 350;

		struct P351 : Packet<P351> {
			// BYTE
			// DWORD
		};
		const DWORD Packet<P351>::STATIC_HEADER = 351;

		struct P352 : Packet<P352> {
			// WORD
			// DWORD
			// int[3] // prefixType="int16"
		};
		const DWORD Packet<P352>::STATIC_HEADER = 352;

		struct P353 : Packet<P353> {
		};
		const DWORD Packet<P353>::STATIC_HEADER = 353;

		struct P354 : Packet<P354> {
		};
		const DWORD Packet<P354>::STATIC_HEADER = 354;

		struct P355 : Packet<P355> {
			// BYTE
		};
		const DWORD Packet<P355>::STATIC_HEADER = 355;

		struct P356 : Packet<P356> {
			// BYTE
		};
		const DWORD Packet<P356>::STATIC_HEADER = 356;

		struct P357 : Packet<P357> {
		};
		const DWORD Packet<P357>::STATIC_HEADER = 357;

		struct P358 : Packet<P358> {
		};
		const DWORD Packet<P358>::STATIC_HEADER = 358;

		struct P359 : Packet<P359> {
			// DWORD
			// BYTE
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// BYTE
			// BYTE
			// BYTE
			// BYTE
			// BYTE
			// WORD
			// WORD
			// WORD
			// WORD
			// WORD
			// DWORD
			// BYTE
			// BYTE
			// DWORD
			// BYTE
		};
		const DWORD Packet<P359>::STATIC_HEADER = 359;

		struct P360 : Packet<P360> {
			// DWORD
		};
		const DWORD Packet<P360>::STATIC_HEADER = 360;

		struct P361 : Packet<P361> {
			// DWORD
		};
		const DWORD Packet<P361>::STATIC_HEADER = 361;

		struct P362 : Packet<P362> {
			// DWORD
			// BYTE
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// DWORD
			// BYTE
			// BYTE
			// BYTE
			// BYTE
			// BYTE
			// WORD
			// WORD
			// WORD
			// WORD
			// WORD
			// DWORD
			// BYTE
			// BYTE
			// DWORD
			// BYTE
		};
		const DWORD Packet<P362>::STATIC_HEADER = 362;

		struct P363 : Packet<P363> {
			// DWORD
			// WORD
			// BYTE
			// wchar_t array[64] // prefixType="int16"
		};
		const DWORD Packet<P363>::STATIC_HEADER = 363;

		struct P364 : Packet<P364> {
			// DWORD
			// WORD
		};
		const DWORD Packet<P364>::STATIC_HEADER = 364;

		struct P365 : Packet<P365> {
			// DWORD
			// WORD
			// BYTE
			// wchar_t array[64] // prefixType="int16"
		};
		const DWORD Packet<P365>::STATIC_HEADER = 365;

		struct P366 : Packet<P366> {
			// WORD
			// wchar_t array[64] // prefixType="int16"
			// wchar_t array[6] // prefixType="int16"
			// WORD
			// BYTE
			// BYTE
			// BYTE
			// BYTE
			// WORD
			// WORD
			// BYTE
			// BYTE
			// BYTE
			// BYTE
			// WORD
			// WORD
			// DWORD
			// DWORD
			// DWORD
			// BYTE
		};
		const DWORD Packet<P366>::STATIC_HEADER = 366;

		struct P367 : Packet<P367> {
			// DWORD
			// WORD
			// BYTE
		};
		const DWORD Packet<P367>::STATIC_HEADER = 367;

		struct P368 : Packet<P368> {
			// BYTE
		};
		const DWORD Packet<P368>::STATIC_HEADER = 368;

		struct P369 : Packet<P369> {
		};
		const DWORD Packet<P369>::STATIC_HEADER = 369;

		struct P370 : Packet<P370> {
			// BYTE
			// BYTE
			// BYTE
			// BYTE
		};
		const DWORD Packet<P370>::STATIC_HEADER = 370;

		struct P371 : Packet<P371> {
			// wchar_t array[20] // prefixType="int16"
		};
		const DWORD Packet<P371>::STATIC_HEADER = 371;

		struct P372 : Packet<P372> {
		};
		const DWORD Packet<P372>::STATIC_HEADER = 372;

		struct P373 : Packet<P373> {
			// DWORD
		};
		const DWORD Packet<P373>::STATIC_HEADER = 373;

		struct P374 : Packet<P374> {
			// wchar_t array[122] // prefixType="int16"
			// BYTE
			// DWORD
			// DWORD
		};
		const DWORD Packet<P374>::STATIC_HEADER = 374;

		struct P375 : Packet<P375> {
			// DWORD
		};
		const DWORD Packet<P375>::STATIC_HEADER = 375;

		struct P376 : Packet<P376> {
			// wchar_t array[122] // prefixType="int16"
			// BYTE
		};
		const DWORD Packet<P376>::STATIC_HEADER = 376;

		struct P377 : Packet<P377> {
			// DWORD
		};
		const DWORD Packet<P377>::STATIC_HEADER = 377;

		struct P378 : Packet<P378> {
			// <Field occurs="16" prefixType="int16" static="true" type="int8"/>
			// wchar_t array[20] // prefixType="int16"
			// WORD
			// <Field occurs="1024" prefixType="int16" type="int8"/>
		};
		const DWORD Packet<P378>::STATIC_HEADER = 378;

		struct P379 : Packet<P379> {
		};
		const DWORD Packet<P379>::STATIC_HEADER = 379;

		struct P380 : Packet<P380> {
		};
		const DWORD Packet<P380>::STATIC_HEADER = 380;

		struct P381 : Packet<P381> {
			// DWORD
		};
		const DWORD Packet<P381>::STATIC_HEADER = 381;

		struct P382 : Packet<P382> {
			// BYTE
		};
		const DWORD Packet<P382>::STATIC_HEADER = 382;

		struct P383 : Packet<P383> {
			// WORD
		};
		const DWORD Packet<P383>::STATIC_HEADER = 383;

		struct P384 : Packet<P384> {
		};
		const DWORD Packet<P384>::STATIC_HEADER = 384;

		struct P385 : Packet<P385> {
			// WORD
			// WORD
			// DWORD
		};
		const DWORD Packet<P385>::STATIC_HEADER = 385;

		struct P386 : Packet<P386> {
			// BYTE
			// DWORD
		};
		const DWORD Packet<P386>::STATIC_HEADER = 386;

		struct P387 : Packet<P387> {
			// WORD
		};
		const DWORD Packet<P387>::STATIC_HEADER = 387;

		struct P388 : Packet<P388> {
			// WORD
			// BYTE
			// BYTE
			// BYTE
			// DWORD
			// wchar_t array[64] // prefixType="int16"
			// <Field occurs="8" prefixType="int8" type="nested">
			// wchar_t array[20] // prefixType="int16"
		};
		const DWORD Packet<P388>::STATIC_HEADER = 388;

		struct P389 : Packet<P389> {
			// WORD
			// BYTE
		};
		const DWORD Packet<P389>::STATIC_HEADER = 389;

		struct P390 : Packet<P390> {
			// BYTE
		};
		const DWORD Packet<P390>::STATIC_HEADER = 390;

		struct P391_InstanceLoadFile : Packet<P391_InstanceLoadFile> {
			DWORD map_fileID;
			Vector2f spawn_point;
			WORD spawn_plane;
			BYTE unk1;
			BYTE unk2;
		};
		const DWORD Packet<P391_InstanceLoadFile>::STATIC_HEADER = 391;

		struct P392 : Packet<P392> {
			// <Field occurs="1024" prefixType="int16" type="int8"/>
		};
		const DWORD Packet<P392>::STATIC_HEADER = 392;

		struct P393 : Packet<P393> {
			// BYTE
			// WORD
			// DWORD
		};
		const DWORD Packet<P393>::STATIC_HEADER = 393;

		struct P394 : Packet<P394> {
			// BYTE
		};
		const DWORD Packet<P394>::STATIC_HEADER = 394;

		struct P395 : Packet<P395> {
			// DWORD agent_id;
			// WORD
			// BYTE
			// DWORD
			// BYTE
			// BYTE
		};
		const DWORD Packet<P395>::STATIC_HEADER = 395;

		struct P396 : Packet<P396> {
			// WORD
		};
		const DWORD Packet<P396>::STATIC_HEADER = 396;

		struct P397 : Packet<P397> {
			// wchar_t array[122] // prefixType="int16"
		};
		const DWORD Packet<P397>::STATIC_HEADER = 397;

		struct P398 : Packet<P398> {
			// DWORD
			// WORD
			// WORD
			// BYTE
			// BYTE
			// BYTE
			// <Field occurs="2" prefixType="int8" type="nested">
			// BYTE
			// BYTE
			// BYTE
			// BYTE
			// BYTE
			// BYTE
			// BYTE
			// WORD
			// BYTE
			// wchar_t array[48] // prefixType="int16"
		};
		const DWORD Packet<P398>::STATIC_HEADER = 398;

		struct P399 : Packet<P399> {
		};
		const DWORD Packet<P399>::STATIC_HEADER = 399;

		struct P400 : Packet<P400> {
			// <Field occurs="16" prefixType="int8" type="nested">
			// WORD
			// DWORD
		};
		const DWORD Packet<P400>::STATIC_HEADER = 400;

		struct P401 : Packet<P401> {
			// BYTE
			// wchar_t array[122] // prefixType="int16"
			// wchar_t array[122] // prefixType="int16"
			// BYTE
			// DWORD
			// DWORD
			// DWORD
			// DWORD
		};
		const DWORD Packet<P401>::STATIC_HEADER = 401;

		struct P402 : Packet<P402> {
			// BYTE
		};
		const DWORD Packet<P402>::STATIC_HEADER = 402;

		struct P403 : Packet<P403> {
			// BYTE
			// BYTE
			// DWORD
			// DWORD
			// DWORD
			// DWORD
		};
		const DWORD Packet<P403>::STATIC_HEADER = 403;

		struct P404 : Packet<P404> {
			// BYTE
			// wchar_t array[122] // prefixType="int16"
			// wchar_t array[122] // prefixType="int16"
		};
		const DWORD Packet<P404>::STATIC_HEADER = 404;

		struct P405 : Packet<P405> {
			// DWORD agent_id;
			// WORD
			// BYTE
			// DWORD
			// BYTE
			// BYTE
			// DWORD
			// Vector2f
			// WORD
			// BYTE
			// BYTE
			// wchar_t array[20] // prefixType="int16"
		};
		const DWORD Packet<P405>::STATIC_HEADER = 405;

		struct P406 : Packet<P406> {
			// <Field occurs="24" prefixType="int16" static="true" type="int8"/>
			// DWORD
			// BYTE
			// WORD
			// BYTE
			// DWORD
		};
		const DWORD Packet<P406>::STATIC_HEADER = 406;

		struct P407 : Packet<P407> {
			// BYTE
			// BYTE
			// DWORD
			// wchar_t array[122] // prefixType="int16"
			// DWORD
			// wchar_t array[122] // prefixType="int16"
			// DWORD
			// BYTE
			// BYTE
		};
		const DWORD Packet<P407>::STATIC_HEADER = 407;

		struct P408 : Packet<P408> {
			// BYTE
		};
		const DWORD Packet<P408>::STATIC_HEADER = 408;

		struct P409 : Packet<P409> {
			// BYTE
			// DWORD
		};
		const DWORD Packet<P409>::STATIC_HEADER = 409;

		struct P410 : Packet<P410> {
			// BYTE
			// DWORD
		};
		const DWORD Packet<P410>::STATIC_HEADER = 410;

		struct P411 : Packet<P411> {
			// BYTE
		};
		const DWORD Packet<P411>::STATIC_HEADER = 411;

		struct P412 : Packet<P412> {
			// DWORD
		};
		const DWORD Packet<P412>::STATIC_HEADER = 412;

		struct P413 : Packet<P413> {
			// WORD
			// DWORD
			// DWORD
			// wchar_t array[20] // prefixType="int16"
		};
		const DWORD Packet<P413>::STATIC_HEADER = 413;

		struct P414 : Packet<P414> {
			// WORD
			// DWORD
			// BYTE
			// BYTE
			// BYTE
			// wchar_t array[20] // prefixType="int16"
		};
		const DWORD Packet<P414>::STATIC_HEADER = 414;

		struct P415 : Packet<P415> {
			// WORD
			// BYTE
			// BYTE
			// BYTE
			// BYTE
			// BYTE
			// WORD
			// BYTE
			// DWORD
			// DWORD
			// wchar_t array[32] // prefixType="int16"
			// wchar_t array[6] // prefixType="int16"
		};
		const DWORD Packet<P415>::STATIC_HEADER = 415;

		struct P416 : Packet<P416> {
			// wchar_t array[122] // prefixType="int16"
		};
		const DWORD Packet<P416>::STATIC_HEADER = 416;

		struct P417 : Packet<P417> {
			// DWORD
		};
		const DWORD Packet<P417>::STATIC_HEADER = 417;

		struct P418 : Packet<P418> {
			// WORD
			// BYTE
			// DWORD
		};
		const DWORD Packet<P418>::STATIC_HEADER = 418;

		struct P419 : Packet<P419> {
			// WORD
			// BYTE
		};
		const DWORD Packet<P419>::STATIC_HEADER = 419;

		struct P420 : Packet<P420> {
			// BYTE
			// BYTE
			// wchar_t array[32] // prefixType="int16"
			// DWORD
			// BYTE
			// DWORD
		};
		const DWORD Packet<P420>::STATIC_HEADER = 420;

		struct P421 : Packet<P421> {
		};
		const DWORD Packet<P421>::STATIC_HEADER = 421;

		struct P422 : Packet<P422> {
			// BYTE
			// BYTE
			// wchar_t array[32] // prefixType="int16"
			// DWORD
			// BYTE
			// DWORD
		};
		const DWORD Packet<P422>::STATIC_HEADER = 422;

		struct P423 : Packet<P423> {
		};
		const DWORD Packet<P423>::STATIC_HEADER = 423;

		struct P424 : Packet<P424> {
			// BYTE
		};
		const DWORD Packet<P424>::STATIC_HEADER = 424;

		struct P425 : Packet<P425> {
			// BYTE
		};
		const DWORD Packet<P425>::STATIC_HEADER = 425;

		struct P426 : Packet<P426> {
		};
		const DWORD Packet<P426>::STATIC_HEADER = 426;

		struct P427 : Packet<P427> {
		};
		const DWORD Packet<P427>::STATIC_HEADER = 427;

		struct P428 : Packet<P428> {
			// WORD
			// BYTE
			// wchar_t array[122] // prefixType="int16"
		};
		const DWORD Packet<P428>::STATIC_HEADER = 428;

		struct P429 : Packet<P429> {
			// BYTE
		};
		const DWORD Packet<P429>::STATIC_HEADER = 429;

		struct P430 : Packet<P430> {
			// DWORD
		};
		const DWORD Packet<P430>::STATIC_HEADER = 430;

		struct P431 : Packet<P431> {
			// BYTE
		};
		const DWORD Packet<P431>::STATIC_HEADER = 431;

		struct P432 : Packet<P432> {
			// WORD
			// WORD
			// wchar_t array[20] // prefixType="int16"
			// BYTE
			// BYTE
		};
		const DWORD Packet<P432>::STATIC_HEADER = 432;

		struct P433 : Packet<P433> {
			// WORD
			// WORD
		};
		const DWORD Packet<P433>::STATIC_HEADER = 433;

		struct P434 : Packet<P434> {
			// WORD
		};
		const DWORD Packet<P434>::STATIC_HEADER = 434;

		struct P435 : Packet<P435> {
			// WORD
			// WORD
			// WORD
			// BYTE
			// BYTE
		};
		const DWORD Packet<P435>::STATIC_HEADER = 435;

		struct P436 : Packet<P436> {
			// WORD
			// WORD
			// WORD
		};
		const DWORD Packet<P436>::STATIC_HEADER = 436;

		struct P437 : Packet<P437> {
			// WORD
		};
		const DWORD Packet<P437>::STATIC_HEADER = 437;

		struct P438 : Packet<P438> {
			// WORD
		};
		const DWORD Packet<P438>::STATIC_HEADER = 438;

		struct P439 : Packet<P439> {
			// WORD
		};
		const DWORD Packet<P439>::STATIC_HEADER = 439;

		struct P440 : Packet<P440> {
			// WORD
		};
		const DWORD Packet<P440>::STATIC_HEADER = 440;

		struct P441 : Packet<P441> {
			// WORD
		};
		const DWORD Packet<P441>::STATIC_HEADER = 441;

		struct P442 : Packet<P442> {
			// WORD
		};
		const DWORD Packet<P442>::STATIC_HEADER = 442;

		struct P443 : Packet<P443> {
			// BYTE
		};
		const DWORD Packet<P443>::STATIC_HEADER = 443;

		struct P444 : Packet<P444> {
			// WORD
			// WORD
			// BYTE
		};
		const DWORD Packet<P444>::STATIC_HEADER = 444;

		struct P445 : Packet<P445> {
			// WORD
			// WORD
		};
		const DWORD Packet<P445>::STATIC_HEADER = 445;

		struct P446 : Packet<P446> {
			// WORD
			// WORD
		};
		const DWORD Packet<P446>::STATIC_HEADER = 446;

		struct P447 : Packet<P447> {
			// WORD
			// WORD
		};
		const DWORD Packet<P447>::STATIC_HEADER = 447;

		struct P448 : Packet<P448> {
			// WORD
			// WORD
		};
		const DWORD Packet<P448>::STATIC_HEADER = 448;

		struct P449 : Packet<P449> {
			// WORD
			// WORD
		};
		const DWORD Packet<P449>::STATIC_HEADER = 449;

		struct P450 : Packet<P450> {
			// WORD
			// WORD
			// BYTE
		};
		const DWORD Packet<P450>::STATIC_HEADER = 450;

		struct P451 : Packet<P451> {
			// WORD
		};
		const DWORD Packet<P451>::STATIC_HEADER = 451;

		struct P452 : Packet<P452> {
			// WORD
		};
		const DWORD Packet<P452>::STATIC_HEADER = 452;

		struct P453 : Packet<P453> {
			// wchar_t array[122] // prefixType="int16"
		};
		const DWORD Packet<P453>::STATIC_HEADER = 453;

		struct P454 : Packet<P454> {
		};
		const DWORD Packet<P454>::STATIC_HEADER = 454;

		struct P455 : Packet<P455> {
			// BYTE
		};
		const DWORD Packet<P455>::STATIC_HEADER = 455;

		struct P456 : Packet<P456> {
			// <Field occurs="16" prefixType="int16" type="int16"/>
			// <Field occurs="16" prefixType="int8" type="nested">
			// DWORD
			// DWORD
			// DWORD
		};
		const DWORD Packet<P456>::STATIC_HEADER = 456;

		struct P457 : Packet<P457> {
		};
		const DWORD Packet<P457>::STATIC_HEADER = 457;

		struct P458 : Packet<P458> {
			// BYTE
			// BYTE
			// wchar_t array[122] // prefixType="int16"
		};
		const DWORD Packet<P458>::STATIC_HEADER = 458;

		struct P459 : Packet<P459> {
			// WORD
			// BYTE
		};
		const DWORD Packet<P459>::STATIC_HEADER = 459;

		struct P460 : Packet<P460> {
			// WORD
			// WORD
		};
		const DWORD Packet<P460>::STATIC_HEADER = 460;

		struct P461 : Packet<P461> {
			// WORD
			// WORD
		};
		const DWORD Packet<P461>::STATIC_HEADER = 461;

		struct P462 : Packet<P462> {
			// WORD
			// WORD
			// BYTE
			// BYTE
			// BYTE
			// BYTE
			// BYTE
			// wchar_t array[32] // prefixType="int16"
			// wchar_t array[20] // prefixType="int16"
			// BYTE
			// BYTE
			// BYTE
			// DWORD
		};
		const DWORD Packet<P462>::STATIC_HEADER = 462;

		struct P463 : Packet<P463> {
			// WORD
		};
		const DWORD Packet<P463>::STATIC_HEADER = 463;

		struct P464 : Packet<P464> {
			// WORD
		};
		const DWORD Packet<P464>::STATIC_HEADER = 464;

		struct P465 : Packet<P465> {
			// WORD
			// BYTE
			// BYTE
		};
		const DWORD Packet<P465>::STATIC_HEADER = 465;

		struct P466 : Packet<P466> {
			// WORD
			// BYTE
			// BYTE
		};
		const DWORD Packet<P466>::STATIC_HEADER = 466;

		struct P467 : Packet<P467> {
			// BYTE
		};
		const DWORD Packet<P467>::STATIC_HEADER = 467;

		struct P468 : Packet<P468> {
			// wchar_t array[122] // prefixType="int16"
		};
		const DWORD Packet<P468>::STATIC_HEADER = 468;

		struct P469 : Packet<P469> {
			// BYTE
		};
		const DWORD Packet<P469>::STATIC_HEADER = 469;

		struct P470 : Packet<P470> {
			// BYTE
		};
		const DWORD Packet<P470>::STATIC_HEADER = 470;

		struct P471 : Packet<P471> {
			// DWORD
		};
		const DWORD Packet<P471>::STATIC_HEADER = 471;

		struct P472 : Packet<P472> {
			// BYTE
		};
		const DWORD Packet<P472>::STATIC_HEADER = 472;

		struct P473 : Packet<P473> {
			// BYTE
			// DWORD
		};
		const DWORD Packet<P473>::STATIC_HEADER = 473;

		struct P474 : Packet<P474> {
			// DWORD
		};
		const DWORD Packet<P474>::STATIC_HEADER = 474;

		struct P475 : Packet<P475> {
			// DWORD
			// BYTE
		};
		const DWORD Packet<P475>::STATIC_HEADER = 475;

		struct P476 : Packet<P476> {
			// DWORD
		};
		const DWORD Packet<P476>::STATIC_HEADER = 476;

		struct P477 : Packet<P477> {
		};
		const DWORD Packet<P477>::STATIC_HEADER = 477;

		struct P478 : Packet<P478> {
		};
		const DWORD Packet<P478>::STATIC_HEADER = 478;

		struct P479 : Packet<P479> {
		};
		const DWORD Packet<P479>::STATIC_HEADER = 479;

		struct P480 : Packet<P480> {
		};
		const DWORD Packet<P480>::STATIC_HEADER = 480;

		struct P481 : Packet<P481> {
			// DWORD
			// DWORD
		};
		const DWORD Packet<P481>::STATIC_HEADER = 481;
	}
}