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

            struct AgentSetPlayer : Packet<AgentSetPlayer> {
				DWORD unk1;
				DWORD unk2;
			};
			const DWORD Packet<AgentSetPlayer>::STATIC_HEADER = 35;

            struct AgentUpdateAllegiance : Packet<AgentUpdateAllegiance> {
                DWORD agent_id;
                DWORD unk1; // more than just allegiance, determines things that change.
            };
            const DWORD Packet<AgentUpdateAllegiance>::STATIC_HEADER = 48;

            // creates the "ping" on an enemy when some player targets it
			struct AgentPinged : Packet<AgentPinged> {
				DWORD Player; // who sent the message
				DWORD agent_id; // target agent
			};
			const DWORD Packet<AgentPinged>::STATIC_HEADER = 53;

            // prety much a bond from someone else / hero bond
            struct AddExternalBond : Packet<AddExternalBond> {
				DWORD caster_id;
				DWORD receiver_id;
				DWORD skill_id;
				DWORD effect_type;
				DWORD effect_id;
			};
			const DWORD Packet<AddExternalBond>::STATIC_HEADER = 65;

            // Display Cape (?)
			struct DisplayCape : Packet<DisplayCape> {
				DWORD agent_id;
				BYTE unk0;
			};
			const DWORD Packet<DisplayCape>::STATIC_HEADER = 72;

			/*
            struct P067 : Packet<P067> {
				// DWORD
			};
            const DWORD Packet<P067>::STATIC_HEADER = 66;
			*/

            struct NpcGeneralStats : Packet<NpcGeneralStats> {
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
			const DWORD Packet<NpcGeneralStats>::STATIC_HEADER = 86;

            // NPC model file (?)
			struct NPCModelFile : Packet<NPCModelFile> {
				DWORD npc_id;
				DWORD count;
				DWORD data[8];
			};
			const DWORD Packet<NPCModelFile>::STATIC_HEADER = 87;

            // Define chat message
			struct MessageCore : Packet<MessageCore> {
				wchar_t message[122]; // prefixType="int16"
			};
			const DWORD Packet<MessageCore>::STATIC_HEADER = 93;

            // Deliver chat message (no owner)
            struct MessageServer : Packet<MessageServer> {
				DWORD id; // some kind of ID of the affected target
				DWORD type; // enum ChatChannel above.
			};
			const DWORD Packet<MessageServer>::STATIC_HEADER = 94;

			/*
            struct P088 : Packet<P088> {
				DWORD agent_id;
				DWORD unk1;
				wchar_t unk2[8]; // prefixType="int16"
			};
			const DWORD Packet<P088>::STATIC_HEADER = 87;
			*/

            // Deliver chat message (player sender in guild or alliance chat)
            struct MessageGlobal : Packet<MessageGlobal> {
				DWORD id; // some kind of ID
				wchar_t sender_name[32]; // full in-game name
				wchar_t sender_guild[6]; // guild tag for alliance chat, empty for guild chat
			};
			const DWORD Packet<MessageGlobal>::STATIC_HEADER = 96;

            // Deliver chat message (player sender in the instance)
			struct MessageLocal : Packet<MessageLocal> {
				DWORD id; // PlayerNumber of the sender
				DWORD type; // enum ChatChannel above.
			};
			const DWORD Packet<MessageLocal>::STATIC_HEADER = 97;

            // Alcohol Post Process Effect
			struct PostProcess : Packet<PostProcess> {
				DWORD level;
				DWORD tint;
			};
			const DWORD Packet<PostProcess>::STATIC_HEADER = 107;

			struct DataWindow : Packet<DataWindow> {
				DWORD agent;
				DWORD type; // 0=storage, 1=tournament, 2=records, 3=stylist
				DWORD data;
			};
			const DWORD Packet<DataWindow>::STATIC_HEADER = 131;

			struct ItemStreamEnd : Packet<ItemStreamEnd> {
				DWORD unk1;
			};
			const DWORD Packet<ItemStreamEnd>::STATIC_HEADER = 134;

            // Pings and drawing in compass
			struct CompassEvent : Packet<CompassEvent> {
				DWORD Player;    // player who sent the ping (PlayerNumber)
				DWORD SessionID; // Changes for different pings/lines/curves
				DWORD NumberPts; // Number of points in the data, between 1 and 8
				struct {
					short x;	 // world coordinates divided by 10
					short y;	 // same
				} points[8];
				// there *might* be another 8 DWORDs, but they look like noise and they are not relayed by the server to other players
			};
			const DWORD Packet<CompassEvent>::STATIC_HEADER = 146;

            struct AgentScale : Packet<AgentScale> {
				DWORD agent_id;
				DWORD scale;
			};
			const DWORD Packet<AgentScale>::STATIC_HEADER = 155;

            // agent animation lock (and probably something else)
			struct GenericValue : Packet<GenericValue> {
				DWORD unk1;
				DWORD agent_id;
				DWORD unk2;
			};
			const DWORD Packet<GenericValue>::STATIC_HEADER = 160;

            // Update Target Generic Value
			struct GenericValueTarget : Packet<GenericValueTarget> {
				DWORD Value_id;
				DWORD target; // agent id
				DWORD caster; // agent id
				DWORD value;
			};
			const DWORD Packet<GenericValueTarget>::STATIC_HEADER = 161;

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
			struct GenericModifier : Packet<GenericModifier> {
				DWORD type;			// type as specified above in P156_Type
				DWORD target_id;	// agent id of who is affected by the change
				DWORD cause_id;		// agent id of who caused the change
				float value;		// value, often in percentage (e.g. %hp)
			};
			const DWORD Packet<GenericModifier>::STATIC_HEADER = 164;

            // agent text above head
			struct SpeechBubble : Packet<SpeechBubble> {
				DWORD agent_id;
				wchar_t message[122];
			};
			const DWORD Packet<SpeechBubble>::STATIC_HEADER = 166;

            // agent change model
			struct AgentModel : Packet<AgentModel> {
				DWORD agent_id;
				DWORD model_id;
			};
			const DWORD Packet<AgentModel>::STATIC_HEADER = 175;

			struct ObjectiveAdd : Packet<ObjectiveAdd> {
				DWORD objective_id;
				DWORD type;
				wchar_t name[128];
			};
			const DWORD Packet<ObjectiveAdd>::STATIC_HEADER = 188;

			struct ObjectiveDone : Packet<ObjectiveDone> {
				DWORD objective_id;
			};
			const DWORD Packet<ObjectiveDone>::STATIC_HEADER = 189;

			struct ObjectiveUpdateName : Packet<ObjectiveUpdateName> {
				DWORD objective_id;
				wchar_t objective_name[128];
			};
			const DWORD Packet<ObjectiveUpdateName>::STATIC_HEADER = 190;

			struct TransactionDone : Packet<TransactionDone> {
				DWORD unk1;
			};
			const DWORD Packet<TransactionDone>::STATIC_HEADER = 205;

            // Skill Activate (begin casting)
			struct SkillActivate : Packet<SkillActivate> {
				DWORD agent_id;
				DWORD skill_id;
				DWORD skill_instance;
			};
			const DWORD Packet<SkillActivate>::STATIC_HEADER = 229;

            // update agent state
			struct AgentState : Packet<AgentState> {
				DWORD agent_id;
				DWORD state; // bitmap of agent states (0 neutral, 2 condition, 128 enchanted, 1024 degen?, 2048 hexed, 8192 sitting, etc)
			};
			const DWORD Packet<AgentState>::STATIC_HEADER = 242;

            struct MapLoaded : Packet<MapLoaded> {
				// DWORD
			};
			const DWORD Packet<MapLoaded>::STATIC_HEADER = 243;

            struct QuotedItemPrice : Packet<QuotedItemPrice> {
				DWORD itemid;
				DWORD price;
			};
			const DWORD Packet<QuotedItemPrice>::STATIC_HEADER = 248;

			struct CinematicPlay : Packet<CinematicPlay> {
				DWORD play;
			};
			const DWORD Packet<CinematicPlay>::STATIC_HEADER = 258;

            // Gold added to inventory
			struct CharacterAddGold : Packet<CharacterAddGold> {
				DWORD unk;
				DWORD gold;
			};
			const DWORD Packet<CharacterAddGold>::STATIC_HEADER = 323;

            // Gold removed from inventory
			struct CharacterRemoveGold : Packet<CharacterRemoveGold> {
				DWORD unk; // some kind of id? but neither agentid nor playerid
				DWORD gold;
			};
			const DWORD Packet<CharacterRemoveGold>::STATIC_HEADER = 338;

            struct InstanceLoadFile : Packet<InstanceLoadFile> {
				DWORD map_fileID;
				Vector2f spawn_point;
				WORD spawn_plane;
				BYTE unk1;
				BYTE unk2;
			};
			const DWORD Packet<InstanceLoadFile>::STATIC_HEADER = 407;

			struct InstanceLoadInfo : Packet<InstanceLoadInfo> {
				DWORD agent_id;
				DWORD map_id;
				DWORD is_explorable;
				DWORD district;
				DWORD language;
				DWORD is_observer;
			};
			const DWORD Packet<InstanceLoadInfo>::STATIC_HEADER = 411;

            struct GameSrvTransfer : Packet<GameSrvTransfer> {
				BYTE host[24]; // ip of the game server
				DWORD token1; // world id
				DWORD region; // BYTE
				DWORD map_id; // BYTE
				DWORD is_explorable; // BYTE
				DWORD token2; // player id
			};
			const DWORD Packet<GameSrvTransfer>::STATIC_HEADER = 422;

			struct DoACompleteZone : Packet<DoACompleteZone> {
				wchar_t message[122];
			};
			const DWORD Packet<DoACompleteZone>::STATIC_HEADER = 432;

            struct PartyPlayerAdd : Packet<PartyPlayerAdd> {
				// WORD
				// WORD
				// BYTE
			};
			const DWORD Packet<PartyPlayerAdd>::STATIC_HEADER = 460;

			struct PartyDefeated : Packet<PartyDefeated> {
			};
			const DWORD Packet<PartyDefeated>::STATIC_HEADER = 473;
        }
	}
}
