#pragma once

#include <Windows.h>

#include <GWCA\GameEntities\Position.h>

/*
Server to client packets, sorted by header

most packets are not filled in, however the list and types of
fields is given as comments

feel free to fill packets, and you can also add a suffix to
the packet name, e.g. P391 -> P391_InstanceLoadMap
*/

namespace GW {
	namespace Packet {
		namespace StoC {
			struct PacketBase {
				DWORD header;
			};

			template <class Specific>
			struct Packet : PacketBase {
			public:
				static const DWORD STATIC_HEADER;
			};

            struct P028 : Packet<P028> {
				DWORD unk1;
				DWORD unk2;
			};
			const DWORD Packet<P028>::STATIC_HEADER = 28;

            // creates the "ping" on an enemy when some player targets it
			struct P046 : Packet<P046> {
				DWORD Player; // who sent the message
				DWORD agent_id; // target agent
			};
			const DWORD Packet<P046>::STATIC_HEADER = 46;

            // prety much a bond from someone else / hero bond
            struct AddExternalBond : Packet<AddExternalBond> {
				DWORD caster_id;
				DWORD receiver_id;
				DWORD skill_id;
				DWORD effect_type;
				DWORD effect_id;
			};
			const DWORD Packet<AddExternalBond>::STATIC_HEADER = 58;

            // Display Cape (?)
			struct P065 : Packet<P065> {
				DWORD agent_id;
				BYTE unk0;
			};
			const DWORD Packet<P065>::STATIC_HEADER = 65;

            struct P067 : Packet<P067> {
				// DWORD
			};
            const DWORD Packet<P067>::STATIC_HEADER = 67;

            struct NpcGeneralStats : Packet<NpcGeneralStats > {
				DWORD npc_id;
				DWORD file_id;
				DWORD data1;
				DWORD scale;
				DWORD data2;
				DWORD flags;
				DWORD profession;
				DWORD level;
				wchar_t name[8];
			};
			const DWORD Packet<NpcGeneralStats >::STATIC_HEADER = 79;

            // NPC model file (?)
			struct P080 : Packet<P080> {
				DWORD npc_id;
				DWORD count;
				DWORD data[8];
			};
			const DWORD Packet<P080>::STATIC_HEADER = 80;

            // Define chat message
			struct P086 : Packet<P086> {
				wchar_t message[122]; // prefixType="int16"
			};
			const DWORD Packet<P086>::STATIC_HEADER = 86;

            // Deliver chat message (no owner)
            struct P087 : Packet<P087> {
				DWORD id; // some kind of ID of the affected target
				DWORD type; // enum ChatChannel above.
			};
			const DWORD Packet<P087>::STATIC_HEADER = 87;

            struct P088 : Packet<P088> {
				DWORD agent_id;
				DWORD unk1;
				wchar_t unk2[8]; // prefixType="int16"
			};
			const DWORD Packet<P088>::STATIC_HEADER = 88;

            // Deliver chat message (player sender in guild or alliance chat)
            struct P089 : Packet<P089> {
				DWORD id; // some kind of ID
				wchar_t sender_name[32]; // full in-game name
				wchar_t sender_guild[6]; // guild tag for alliance chat, empty for guild chat
			};
			const DWORD Packet<P089>::STATIC_HEADER = 89;

            // Deliver chat message (player sender in the instance)
			struct P090 : Packet<P090> {
				DWORD id; // PlayerNumber of the sender
				DWORD type; // enum ChatChannel above.
			};
			const DWORD Packet<P090>::STATIC_HEADER = 90;

            // Alcohol Post Process Effect
			struct P100 : Packet<P100> {
				DWORD level;
				DWORD tint;
			};
			const DWORD Packet<P100>::STATIC_HEADER = 100;

            // Pings and drawing in compass
			struct P138 : Packet<P138> {
				DWORD Player;    // player who sent the ping (PlayerNumber)
				DWORD SessionID; // Changes for different pings/lines/curves
				DWORD NumberPts; // Number of points in the data, between 1 and 8
				struct {
					short x;	 // world coordinates divided by 10
					short y;	 // same
				} points[8];
				// there *might* be another 8 DWORDs, but they look like noise and they are not relayed by the server to other players
			};
			const DWORD Packet<P138>::STATIC_HEADER = 138;

            struct P147 : Packet<P147> {
				DWORD agent_id;
				DWORD scale;
			};
			const DWORD Packet<P147>::STATIC_HEADER = 147;

            // agent animation lock (and probably something else)
			struct P152 : Packet<P152> {
				DWORD unk1;
				DWORD agent_id;
				DWORD unk2;
			};
			const DWORD Packet<P152>::STATIC_HEADER = 152;

            // Update Target Generic Value
			struct P153 : Packet<P153> {
				DWORD Value_id;
				DWORD target; // agent id
				DWORD caster; // agent id
				DWORD value;
			};
			const DWORD Packet<P153>::STATIC_HEADER = 153;

            // damage or healing done packet, but also has other purposes.
			// to be investigated further.
			// all types have their value in the float field 'value'.
			// in all types the value is in percentage, unless otherwise specified.
			// the value can be negative (e.g. damage, sacrifice) 
			// or positive (e.g. heal, lifesteal).
			namespace P156_Type {
				const DWORD damage = 16; // non-armor-ignoring attack, spells
				const DWORD critical = 17; // critical hit on autoattack
				const DWORD energygain = 52; // for example from Critical Strikes or energy tap
				const DWORD armorignoring = 55; // all armor ignoring damage and heals
				const DWORD casttime = 61; // non-standard cast time, value in seconds
			}
			struct P156 : Packet<P156> {
				DWORD type;			// type as specified above in P156_Type
				DWORD target_id;	// agent id of who is affected by the change
				DWORD cause_id;		// agent id of who caused the change
				float value;		// value, often in percentage (e.g. %hp)
			};
			const DWORD Packet<P156>::STATIC_HEADER = 156;

            // agent text above head
			struct P158 : Packet<P158> {
				DWORD agent_id;
				wchar_t message[122];
			};
			const DWORD Packet<P158>::STATIC_HEADER = 158;

            // agent change model
			struct P167 : Packet<P167> {
				DWORD agent_id;
				DWORD model_id;
			};
			const DWORD Packet<P167>::STATIC_HEADER = 167;

            // Skill Activate (begin casting)
			struct P221 : Packet<P221> {
				DWORD agent_id;
				DWORD skill_id;
				DWORD skill_instance;
			};
			const DWORD Packet<P221>::STATIC_HEADER = 221;

            // update agent state
			struct P234 : Packet<P234> {
				DWORD agent_id;
				DWORD state; // bitmap of agent states (0 neutral, 2 condition, 128 enchanted, 1024 degen?, 2048 hexed, 8192 sitting, etc)
			};
			const DWORD Packet<P234>::STATIC_HEADER = 234;

            struct P235 : Packet<P235> {
				// DWORD
			};
			const DWORD Packet<P235>::STATIC_HEADER = 235;

            struct QuotedItemPrice : Packet<QuotedItemPrice> {
				DWORD itemid;
				DWORD price;
			};
			const DWORD Packet<QuotedItemPrice>::STATIC_HEADER = 240;

            // Gold added to inventory
			struct P315 : Packet<P315> {
				DWORD unk;
				DWORD gold;
			};
			const DWORD Packet<P315>::STATIC_HEADER = 315;

            // Gold removed from inventory
			struct P330 : Packet<P330> {
				DWORD unk; // some kind of id? but neither agentid nor playerid
				DWORD gold;
			};
			const DWORD Packet<P330>::STATIC_HEADER = 330;

            struct InstanceLoadFile : Packet<InstanceLoadFile> {
				DWORD map_fileID;
				Vector2f spawn_point;
				WORD spawn_plane;
				BYTE unk1;
				BYTE unk2;
			};
			const DWORD Packet<InstanceLoadFile>::STATIC_HEADER = 398;

            struct GameSrvTransfer : Packet<GameSrvTransfer> {
				BYTE host[24]; // ip of the game server
				DWORD token1; // world id
				DWORD region; // BYTE
				DWORD map_id; // BYTE
				DWORD is_explorable; // BYTE
				DWORD token2; // player id
			};
			const DWORD Packet<GameSrvTransfer>::STATIC_HEADER = 413;

            struct P451 : Packet<P451> {
				// WORD
				// WORD
				// BYTE
			};
			const DWORD Packet<P451>::STATIC_HEADER = 451;
        }
	}
}