#pragma once

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
                uint32_t header;
            };

            template <class Specific>
            struct Packet : PacketBase {
            public:
                static const uint32_t STATIC_HEADER;
            };

            struct AgentSetPlayer : Packet<AgentSetPlayer> {
                uint32_t unk1;
                uint32_t unk2;
            };
            const uint32_t Packet<AgentSetPlayer>::STATIC_HEADER = 35;

            struct AgentUpdateAllegiance : Packet<AgentUpdateAllegiance> {
                uint32_t agent_id;
                uint32_t unk1; // more than just allegiance, determines things that change.
            };
            const uint32_t Packet<AgentUpdateAllegiance>::STATIC_HEADER = 48;

            // creates the "ping" on an enemy when some player targets it
            struct AgentPinged : Packet<AgentPinged> {
                uint32_t Player; // who sent the message
                uint32_t agent_id; // target agent
            };
            const uint32_t Packet<AgentPinged>::STATIC_HEADER = 53;

            // prety much a bond from someone else / hero bond
            struct AddExternalBond : Packet<AddExternalBond> {
                uint32_t caster_id;
                uint32_t receiver_id;
                uint32_t skill_id;
                uint32_t effect_type;
                uint32_t effect_id;
            };
            const uint32_t Packet<AddExternalBond>::STATIC_HEADER = 65;

            // Display Cape (?)
            struct DisplayCape : Packet<DisplayCape> {
                uint32_t agent_id;
                uint8_t unk0;
            };
            const uint32_t Packet<DisplayCape>::STATIC_HEADER = 72;

            /*
            struct P067 : Packet<P067> {
                // uint32_t
            };
            const uint32_t Packet<P067>::STATIC_HEADER = 66;
            */

            struct NpcGeneralStats : Packet<NpcGeneralStats> {
                uint32_t npc_id;
                uint32_t file_id;
                uint32_t data1;
                uint32_t scale;
                uint32_t data2;
                uint32_t flags;
                uint32_t profession;
                uint32_t level;
                wchar_t name[8];
            };
            const uint32_t Packet<NpcGeneralStats>::STATIC_HEADER = 86;

            // NPC model file (?)
            struct NPCModelFile : Packet<NPCModelFile> {
                uint32_t npc_id;
                uint32_t count;
                uint32_t data[8];
            };
            const uint32_t Packet<NPCModelFile>::STATIC_HEADER = 87;

            // Define chat message
            struct MessageCore : Packet<MessageCore> {
                wchar_t message[122]; // prefixType="int16"
            };
            const uint32_t Packet<MessageCore>::STATIC_HEADER = 93;

            // Deliver chat message (no owner)
            struct MessageServer : Packet<MessageServer> {
                uint32_t id; // some kind of ID of the affected target
                uint32_t type; // enum ChatChannel above.
            };
            const uint32_t Packet<MessageServer>::STATIC_HEADER = 94;

            /*
            struct P088 : Packet<P088> {
                uint32_t agent_id;
                uint32_t unk1;
                wchar_t unk2[8]; // prefixType="int16"
            };
            const uint32_t Packet<P088>::STATIC_HEADER = 87;
            */

            // Deliver chat message (player sender in guild or alliance chat)
            struct MessageGlobal : Packet<MessageGlobal> {
                uint32_t id; // some kind of ID
                wchar_t sender_name[32]; // full in-game name
                wchar_t sender_guild[6]; // guild tag for alliance chat, empty for guild chat
            };
            const uint32_t Packet<MessageGlobal>::STATIC_HEADER = 96;

            // Deliver chat message (player sender in the instance)
            struct MessageLocal : Packet<MessageLocal> {
                uint32_t id; // PlayerNumber of the sender
                uint32_t type; // enum ChatChannel above.
            };
            const uint32_t Packet<MessageLocal>::STATIC_HEADER = 97;

            // Alcohol Post Process Effect
            struct PostProcess : Packet<PostProcess> {
                uint32_t level;
                uint32_t tint;
            };
            const uint32_t Packet<PostProcess>::STATIC_HEADER = 107;

            struct DataWindow : Packet<DataWindow> {
                uint32_t agent;
                uint32_t type; // 0=storage, 1=tournament, 2=records, 3=stylist
                uint32_t data;
            };
            const uint32_t Packet<DataWindow>::STATIC_HEADER = 131;

            struct ItemStreamEnd : Packet<ItemStreamEnd> {
                uint32_t unk1;
            };
            const uint32_t Packet<ItemStreamEnd>::STATIC_HEADER = 134;

            // Pings and drawing in compass
            struct CompassEvent : Packet<CompassEvent> {
                uint32_t Player;    // player who sent the ping (PlayerNumber)
                uint32_t SessionID; // Changes for different pings/lines/curves
                uint32_t NumberPts; // Number of points in the data, between 1 and 8
                struct {
                    short x;     // world coordinates divided by 10
                    short y;     // same
                } points[8];
                // there *might* be another 8 uint32_ts, but they look like noise and they are not relayed by the server to other players
            };
            const uint32_t Packet<CompassEvent>::STATIC_HEADER = 146;

            struct AgentScale : Packet<AgentScale> {
                uint32_t agent_id;
                uint32_t scale;
            };
            const uint32_t Packet<AgentScale>::STATIC_HEADER = 155;

            // agent animation lock (and probably something else)
            struct GenericValue : Packet<GenericValue> {
                uint32_t unk1;
                uint32_t agent_id;
                uint32_t unk2;
            };
            const uint32_t Packet<GenericValue>::STATIC_HEADER = 160;

            // Update Target Generic Value
            struct GenericValueTarget : Packet<GenericValueTarget> {
                uint32_t Value_id;
                uint32_t target; // agent id
                uint32_t caster; // agent id
                uint32_t value;
            };
            const uint32_t Packet<GenericValueTarget>::STATIC_HEADER = 161;

            // damage or healing done packet, but also has other purposes.
            // to be investigated further.
            // all types have their value in the float field 'value'.
            // in all types the value is in percentage, unless otherwise specified.
            // the value can be negative (e.g. damage, sacrifice) 
            // or positive (e.g. heal, lifesteal).
            namespace P156_Type {
                const uint32_t damage = 16; // non-armor-ignoring attack, spells
                const uint32_t critical = 17; // critical hit on autoattack
                const uint32_t energygain = 52; // for example from Critical Strikes or energy tap
                const uint32_t armorignoring = 55; // all armor ignoring damage and heals
                const uint32_t casttime = 61; // non-standard cast time, value in seconds
            }
            struct GenericModifier : Packet<GenericModifier> {
                uint32_t type;         // type as specified above in P156_Type
                uint32_t target_id;    // agent id of who is affected by the change
                uint32_t cause_id;     // agent id of who caused the change
                float value;        // value, often in percentage (e.g. %hp)
            };
            const uint32_t Packet<GenericModifier>::STATIC_HEADER = 164;

            // agent text above head
            struct SpeechBubble : Packet<SpeechBubble> {
                uint32_t agent_id;
                wchar_t message[122];
            };
            const uint32_t Packet<SpeechBubble>::STATIC_HEADER = 166;

            // agent change model
            struct AgentModel : Packet<AgentModel> {
                uint32_t agent_id;
                uint32_t model_id;
            };
            const uint32_t Packet<AgentModel>::STATIC_HEADER = 175;

            struct ObjectiveAdd : Packet<ObjectiveAdd> {
                uint32_t objective_id;
                uint32_t type;
                wchar_t name[128];
            };
            const uint32_t Packet<ObjectiveAdd>::STATIC_HEADER = 188;

            struct ObjectiveDone : Packet<ObjectiveDone> {
                uint32_t objective_id;
            };
            const uint32_t Packet<ObjectiveDone>::STATIC_HEADER = 189;

            struct ObjectiveUpdateName : Packet<ObjectiveUpdateName> {
                uint32_t objective_id;
                wchar_t objective_name[128];
            };
            const uint32_t Packet<ObjectiveUpdateName>::STATIC_HEADER = 190;

            struct TransactionDone : Packet<TransactionDone> {
                uint32_t unk1;
            };
            const uint32_t Packet<TransactionDone>::STATIC_HEADER = 205;

            // Skill Activate (begin casting)
            struct SkillActivate : Packet<SkillActivate> {
                uint32_t agent_id;
                uint32_t skill_id;
                uint32_t skill_instance;
            };
            const uint32_t Packet<SkillActivate>::STATIC_HEADER = 229;

            // update agent state
            struct AgentState : Packet<AgentState> {
                uint32_t agent_id;
                uint32_t state; // bitmap of agent states (0 neutral, 2 condition, 128 enchanted, 1024 degen?, 2048 hexed, 8192 sitting, etc)
            };
            const uint32_t Packet<AgentState>::STATIC_HEADER = 242;

            struct MapLoaded : Packet<MapLoaded> {
                // uint32_t
            };
            const uint32_t Packet<MapLoaded>::STATIC_HEADER = 243;

            struct QuotedItemPrice : Packet<QuotedItemPrice> {
                uint32_t itemid;
                uint32_t price;
            };
            const uint32_t Packet<QuotedItemPrice>::STATIC_HEADER = 248;

            struct CinematicPlay : Packet<CinematicPlay> {
                uint32_t play;
            };
            const uint32_t Packet<CinematicPlay>::STATIC_HEADER = 258;

            // Gold added to inventory
            struct CharacterAddGold : Packet<CharacterAddGold> {
                uint32_t unk;
                uint32_t gold;
            };
            const uint32_t Packet<CharacterAddGold>::STATIC_HEADER = 323;

            // Gold removed from inventory
            struct CharacterRemoveGold : Packet<CharacterRemoveGold> {
                uint32_t unk; // some kind of id? but neither agentid nor playerid
                uint32_t gold;
            };
            const uint32_t Packet<CharacterRemoveGold>::STATIC_HEADER = 338;

            struct InstanceLoadFile : Packet<InstanceLoadFile> {
                uint32_t map_fileID;
                Vec2f spawn_point;
                uint16_t spawn_plane;
                uint8_t unk1;
                uint8_t unk2;
            };
            const uint32_t Packet<InstanceLoadFile>::STATIC_HEADER = 407;

            struct InstanceLoadInfo : Packet<InstanceLoadInfo> {
                uint32_t agent_id;
                uint32_t map_id;
                uint32_t is_explorable;
                uint32_t district;
                uint32_t language;
                uint32_t is_observer;
            };
            const uint32_t Packet<InstanceLoadInfo>::STATIC_HEADER = 411;

            struct GameSrvTransfer : Packet<GameSrvTransfer> {
                uint8_t host[24]; // ip of the game server
                uint32_t token1; // world id
                uint32_t region; // uint8_t
                uint32_t map_id; // uint8_t
                uint32_t is_explorable; // uint8_t
                uint32_t token2; // player id
            };
            const uint32_t Packet<GameSrvTransfer>::STATIC_HEADER = 422;

            struct DoACompleteZone : Packet<DoACompleteZone> {
                wchar_t message[122];
            };
            const uint32_t Packet<DoACompleteZone>::STATIC_HEADER = 432;

            struct PartyPlayerAdd : Packet<PartyPlayerAdd> {
                // uint16_t
                // uint16_t
                // uint8_t
            };
            const uint32_t Packet<PartyPlayerAdd>::STATIC_HEADER = 460;

            struct PartyDefeated : Packet<PartyDefeated> {
            };
            const uint32_t Packet<PartyDefeated>::STATIC_HEADER = 473;
        }
    }
}
