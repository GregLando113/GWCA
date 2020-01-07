#pragma once

#include <GWCA/GameContainers/GamePos.h>

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
            struct TradeStart : Packet<TradeStart> {
                uint32_t player_number;
            };
            const uint32_t Packet<TradeStart>::STATIC_HEADER = 0x0;

            struct TradeCancel : Packet<TradeCancel> {
                uint32_t player_number;
            };
            const uint32_t Packet<TradeCancel>::STATIC_HEADER = 0x1;

            struct Ping : Packet<Ping> {
                uint32_t ping;
            };
            const uint32_t Packet<Ping>::STATIC_HEADER = 0xC;

            struct InstanceTimer : Packet<InstanceTimer> {
                uint32_t instance_time;
            };
            const uint32_t Packet<InstanceTimer>::STATIC_HEADER = 0x20;

            // Called when the client needs to add an agent to memory (i.e. agent appeared within compass range)
            struct AgentAdd : Packet<AgentAdd> {
                uint32_t agent_id;
                uint32_t agent_type; // Bitwise field. 0x20000000 = NPC | PlayerNumber, 0x30000000 = Player | PlayerNumber, 0x00000000 = Signpost
                uint32_t unk2; // byte, agent_type > 0 ? 1 : 4
                uint32_t unk3; // byte
                Vec2f position;
                uint32_t unk4; // word
                Vec2f unk5;
                uint32_t unk6; // word
                float speed; // default 288.0
                float unk7; // default 1.0
                uint32_t unknown_bitwise_1;
                uint32_t allegiance_bits;
                uint32_t unk8[5];
                Vec2f unk9;
                Vec2f unk10; // inf, inf
                uint32_t unk11[2];
                Vec2f unk12; // inf, inf
                uint32_t unk13;
            };
            const uint32_t Packet<AgentAdd>::STATIC_HEADER = 0x21;

            // Called when the client needs to remove an agent from memory (e.g. out of range)
            struct AgentRemove : Packet<AgentRemove> {
                uint32_t agent_id;
            };
            const uint32_t Packet<AgentRemove>::STATIC_HEADER = 0x22;

            struct AgentSetPlayer : Packet<AgentSetPlayer> {
                uint32_t unk1;
                uint32_t unk2;
            };
            const uint32_t Packet<AgentSetPlayer>::STATIC_HEADER = 0x23;

            struct AgentUpdateAllegiance : Packet<AgentUpdateAllegiance> {
                uint32_t agent_id;
                uint32_t allegiance_bits; // more than just allegiance, determines things that change.
            };
            const uint32_t Packet<AgentUpdateAllegiance>::STATIC_HEADER = 0x30;

            // creates the "ping" on an enemy when some player targets it
            struct AgentPinged : Packet<AgentPinged> {
                uint32_t Player; // who sent the message
                uint32_t agent_id; // target agent
            };
            const uint32_t Packet<AgentPinged>::STATIC_HEADER = 0x35;

            struct PartyRemoveAlly : Packet<PartyRemoveAlly> {
                uint32_t agent_id; // target agent
            };
            const uint32_t Packet<PartyRemoveAlly>::STATIC_HEADER = 0x3E;

            // prety much a bond from someone else / hero bond
            struct AddExternalBond : Packet<AddExternalBond> {
                uint32_t caster_id;
                uint32_t receiver_id;
                uint32_t skill_id;
                uint32_t effect_type;
                uint32_t effect_id;
            };
            const uint32_t Packet<AddExternalBond>::STATIC_HEADER = 0x41;

            // Display Cape (?)
            struct DisplayCape : Packet<DisplayCape> {
                uint32_t agent_id;
                uint8_t unk0;
            };
            const uint32_t Packet<DisplayCape>::STATIC_HEADER = 0x48;

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
            const uint32_t Packet<NpcGeneralStats>::STATIC_HEADER = 0x56;

            // NPC model file (?)
            struct NPCModelFile : Packet<NPCModelFile> {
                uint32_t npc_id;
                uint32_t count;
                uint32_t data[8];
            };
            const uint32_t Packet<NPCModelFile>::STATIC_HEADER = 0x57;

            struct PlayerJoinInstance : Packet<PlayerJoinInstance> {
                uint32_t player_number;
                uint32_t agent_id;
                uint32_t file_id1; // dword
                uint32_t secondary_profession; // byte
                uint32_t unk3; // dword
                uint32_t file_id2; // dword
                wchar_t player_name[32];
            };
            const uint32_t Packet<PlayerJoinInstance>::STATIC_HEADER = 0x59;

            struct PlayerLeaveInstance : Packet<PlayerLeaveInstance> {
                uint32_t player_number;
            };
            const uint32_t Packet<PlayerLeaveInstance>::STATIC_HEADER = 0x5A;

            // Define chat message
            struct MessageCore : Packet<MessageCore> {
                wchar_t message[122]; // prefixType="int16"
            };
            const uint32_t Packet<MessageCore>::STATIC_HEADER = 0x5D;

            // Deliver chat message (no owner)
            struct MessageServer : Packet<MessageServer> {
                uint32_t id; // some kind of ID of the affected target
                uint32_t channel; // enum ChatChannel above.
            };
            const uint32_t Packet<MessageServer>::STATIC_HEADER = 0x5E;

            // Deliver chat message (sender is an NPC)
            struct MessageNPC : Packet<MessageNPC> {
                uint32_t agent_id;
                uint32_t channel; // enum ChatChannel above.
                wchar_t sender_name[8];
            };
            const uint32_t Packet<MessageNPC>::STATIC_HEADER = 0x5F;

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
                uint32_t channel; // enum ChatChannel above.
                wchar_t sender_name[32]; // full in-game name
                wchar_t sender_guild[6]; // guild tag for alliance chat, empty for guild chat
            };
            const uint32_t Packet<MessageGlobal>::STATIC_HEADER = 0x60;

            // Deliver chat message (player sender in the instance)
            struct MessageLocal : Packet<MessageLocal> {
                uint32_t player_number; // PlayerNumber of the sender
                uint32_t channel; // enum ChatChannel above.
            };
            const uint32_t Packet<MessageLocal>::STATIC_HEADER = 0x61;

            // Alcohol Post Process Effect
            struct PostProcess : Packet<PostProcess> {
                uint32_t level;
                uint32_t tint;
            };
            const uint32_t Packet<PostProcess>::STATIC_HEADER = 0x6B;

            struct DungeonReward : Packet<DungeonReward> {
                uint32_t experience;
                uint32_t gold;
                uint32_t skill_points;
            };
            const uint32_t Packet<DungeonReward>::STATIC_HEADER = 0x6C;

            /*struct AgentUnk1 : Packet<AgentUnk1> {
                uint32_t agent_id;
                uint32_t unk1;
            };
            const uint32_t Packet<AgentUnk1>::STATIC_HEADER = 0x6C;*/

            struct AgentUnk2 : Packet<AgentUnk2> {
                uint32_t agent_id;
                uint32_t unk1; // 1 = minipet, 2 = Ally?, 3 = summon
                uint32_t unk2; // always 0
            };
            const uint32_t Packet<AgentUnk2>::STATIC_HEADER = 0x6D;

            struct DialogButton : Packet<DialogButton> {
                uint32_t button_icon; // byte
                wchar_t message[8];
                uint32_t dialog_id;
                uint32_t skill_id; // Default 0xFFFFFFF
            };
            const uint32_t Packet<DialogButton>::STATIC_HEADER = 0x7E;

            struct DialogBody : Packet<DialogBody> {
                wchar_t message[122];
            };
            const uint32_t Packet<DialogBody>::STATIC_HEADER = 0x80;

            struct DialogSender : Packet<DialogSender> {
                uint32_t agent_id;
            };
            const uint32_t Packet<DialogSender>::STATIC_HEADER = 0x81;

            struct DataWindow : Packet<DataWindow> {
                uint32_t agent;
                uint32_t type; // 0=storage, 1=tournament, 2=records, 3=stylist
                uint32_t data;
            };
            const uint32_t Packet<DataWindow>::STATIC_HEADER = 0x83;

            struct WindowItems : Packet<WindowItems> {
                uint32_t count;
                uint32_t item_ids[16];
            };
            const uint32_t Packet<WindowItems>::STATIC_HEADER = 0x84;

            struct WindowItemsEnd : Packet<WindowItemsEnd> {
                uint32_t unk1;
            };
            const uint32_t Packet<WindowItemsEnd>::STATIC_HEADER = 0x85;

            struct ItemStreamEnd : Packet<ItemStreamEnd> { // AKA ItemPricesEnd
                uint32_t unk1;
            };
            const uint32_t Packet<ItemStreamEnd>::STATIC_HEADER = 0x86;

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
            const uint32_t Packet<CompassEvent>::STATIC_HEADER = 0x92;

            struct AgentScale : Packet<AgentScale> {
                uint32_t agent_id;
                uint32_t scale;
            };
            const uint32_t Packet<AgentScale>::STATIC_HEADER = 0x9B;

            struct AgentName : Packet<AgentName> {
                uint32_t agent_id;
                wchar_t name_enc[8];
            };
            const uint32_t Packet<AgentName>::STATIC_HEADER = 0x9C;

            struct DisplayDialogue : Packet<DisplayDialogue> {
                uint32_t agent_id;
                wchar_t name[32];
                uint32_t type;
                wchar_t message[122];
            };
            const uint32_t Packet<DisplayDialogue>::STATIC_HEADER = 0x9F;

            // agent animation lock (and probably something else)
            struct GenericValue : Packet<GenericValue> {
                uint32_t Value_id;
                uint32_t agent_id;
                uint32_t value;
            };
            const uint32_t Packet<GenericValue>::STATIC_HEADER = 0xA0;

            // Update Target Generic Value
            struct GenericValueTarget : Packet<GenericValueTarget> {
                uint32_t Value_id;
                uint32_t target; // agent id
                uint32_t caster; // agent id
                uint32_t value;
            };
            const uint32_t Packet<GenericValueTarget>::STATIC_HEADER = 0xA1;

            // Update Target Generic Value
            struct PlayEffect : Packet<PlayEffect> {
                Vec2f coords;
                uint32_t plane;
                uint32_t agent_id;
                uint32_t effect_id;
                uint32_t data5;
                uint32_t data6;
            };
            const uint32_t Packet<PlayEffect>::STATIC_HEADER = 0xA2;

            // agent animation lock (and probably something else)
            /*struct GenericFloat : Packet<GenericFloat> {
                uint32_t unk1;
                uint32_t agent_id;
                uint32_t unk2;
            };
            const uint32_t Packet<GenericFloat>::STATIC_HEADER = 0xA2;*/

            // Update Target Generic Value
            struct GenericValue2 : Packet<GenericValue2> {
                uint32_t unk1;
                uint32_t agent_id;
                uint32_t unk2;
            };
            const uint32_t Packet<GenericValue2>::STATIC_HEADER = 0xA3;

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
            const uint32_t Packet<GenericModifier>::STATIC_HEADER = 0xA4;

            // agent text above head
            struct SpeechBubble : Packet<SpeechBubble> {
                uint32_t agent_id;
                wchar_t message[122];
            };
            const uint32_t Packet<SpeechBubble>::STATIC_HEADER = 0xA6;

            struct Packet167 : Packet<Packet167> {
                uint32_t agent_id;
                uint32_t unk2;
            };
            const uint32_t Packet<Packet167>::STATIC_HEADER = 0xA7;

            struct Packet48 : Packet<Packet48> {
                uint32_t agent_id;
                uint32_t modifier_1;
            };
            const uint32_t Packet<Packet48>::STATIC_HEADER = 48;

            struct PartyAllyAdd : Packet<PartyAllyAdd> { // When an NPC is added as an ally to your party.
                uint32_t agent_id;
                uint32_t allegiance_bits;
                uint32_t agent_type; // Bitwise field. 0x20000000 = NPC | PlayerNumber, 0x30000000 = Player | PlayerNumber, 0x00000000 = Signpost
            };
            const uint32_t Packet<PartyAllyAdd>::STATIC_HEADER = 0xAB;

            // agent change model
            struct AgentModel : Packet<AgentModel> {
                uint32_t agent_id;
                uint32_t model_id;
            };
            const uint32_t Packet<AgentModel>::STATIC_HEADER = 0xAF;

            // Changes the number above the player's head when leading a party
            struct PartyUpdateSize : Packet<PartyUpdateSize> {
                uint32_t player_id;
                uint32_t size;
            };
            const uint32_t Packet<PartyUpdateSize>::STATIC_HEADER = 0xB1;

            struct ObjectiveAdd : Packet<ObjectiveAdd> {
                uint32_t objective_id;
                uint32_t type;
                wchar_t name[128];
            };
            const uint32_t Packet<ObjectiveAdd>::STATIC_HEADER = 0xBC;

            struct ObjectiveDone : Packet<ObjectiveDone> {
                uint32_t objective_id;
            };
            const uint32_t Packet<ObjectiveDone>::STATIC_HEADER = 0xBD;

            struct ObjectiveUpdateName : Packet<ObjectiveUpdateName> {
                uint32_t objective_id;
                wchar_t objective_name[128];
            };
            const uint32_t Packet<ObjectiveUpdateName>::STATIC_HEADER = 0xBE;

            struct OpenMerchantWindow : Packet<OpenMerchantWindow> {
                uint32_t type;
                uint32_t unk;
            };
            const uint32_t Packet<OpenMerchantWindow>::STATIC_HEADER = 0xC4;

            struct WindowOwner : Packet<WindowOwner> {
                uint32_t agent_id;
            };
            const uint32_t Packet<WindowOwner>::STATIC_HEADER = 0xC5;

            struct TransactionDone : Packet<TransactionDone> {
                uint32_t unk1;
            };
            const uint32_t Packet<TransactionDone>::STATIC_HEADER = 0xCD;

            // Skill available in skills window. Signet of capture can have count > 1
            struct UpdateSkillCount : Packet<UpdateSkillCount> {
                uint32_t skill_id;
                uint32_t count;
            };
            const uint32_t Packet<UpdateSkillCount>::STATIC_HEADER = 0xDD;

            // Skill Activate (begin casting)
            struct SkillActivate : Packet<SkillActivate> {
                uint32_t agent_id;
                uint32_t skill_id;
                uint32_t skill_instance;
            };
            const uint32_t Packet<SkillActivate>::STATIC_HEADER = 0xE5;

            struct SkillRecharge : Packet<SkillRecharge> {
                uint32_t agent_id;
                uint32_t skill_id;
                uint32_t skill_instance;
                uint32_t recharge;
            };
            const uint32_t Packet<SkillRecharge>::STATIC_HEADER = 0xE6;

            struct ModifyFaction : Packet<ModifyFaction> {
                uint32_t faction_type;
                uint32_t amount;
            };
            const uint32_t Packet<ModifyFaction>::STATIC_HEADER = 0xEF;

            // update agent state
            struct AgentState : Packet<AgentState> {
                uint32_t agent_id;
                uint32_t state; // bitmap of agent states (0 neutral, 2 condition, 16 dead, 128 enchanted, 1024 degen?, 2048 hexed, 8192 sitting, etc)
            };
            const uint32_t Packet<AgentState>::STATIC_HEADER = 0xF2;

            struct MapLoaded : Packet<MapLoaded> {
                // uint32_t
            };
            const uint32_t Packet<MapLoaded>::STATIC_HEADER = 0xF3;

            struct QuotedItemPrice : Packet<QuotedItemPrice> {
                uint32_t itemid;
                uint32_t price;
            };
            const uint32_t Packet<QuotedItemPrice>::STATIC_HEADER = 0xF8;

            struct WindowPrices : Packet<WindowPrices> {
                uint32_t count;
                uint32_t item_ids[16];
            };
            const uint32_t Packet<WindowPrices>::STATIC_HEADER = 0xFA;

            struct VanquishProgress : Packet<VanquishProgress> {
                uint32_t foes_killed;
                uint32_t foes_remaining;
            };
            const uint32_t Packet<VanquishProgress>::STATIC_HEADER = 0xFB;

            struct VanquishComplete : Packet<VanquishComplete> {
                uint32_t map_id;
                uint32_t experience;
                uint32_t gold;
            };
            const uint32_t Packet<VanquishComplete>::STATIC_HEADER = 0xFC;

            struct CinematicPlay : Packet<CinematicPlay> {
                uint32_t play;
            };
            const uint32_t Packet<CinematicPlay>::STATIC_HEADER = 0x102;

            // e.g. map doors start opening or closing. AKA "update object animation"
            struct ManipulateMapObject : Packet<ManipulateMapObject> {
                uint32_t object_id; // Door ID
                uint32_t animation_type; // (3 = door closing, 9 = ???, 16 = door opening)
                uint32_t animation_stage; // (2 = start, 3 = stop)
            };
            const uint32_t Packet<ManipulateMapObject>::STATIC_HEADER = 0x111;

            // e.g. map doors stop opening or closing. "update object state"
            struct ManipulateMapObject2 : Packet<ManipulateMapObject2> {
                uint32_t object_id; // Door ID
                uint32_t unk1; // 
                uint32_t state; // Open = 1, Closed = 0
            };
            const uint32_t Packet<ManipulateMapObject2>::STATIC_HEADER = 0x114;

            struct TownAllianceObject : Packet<TownAllianceObject> {
                uint32_t map_id;
                uint32_t rank;
                uint32_t allegiance;
                uint32_t faction;
                wchar_t name[32];
                wchar_t tag[5];
                uint32_t cape_bg_color;
                uint32_t cape_detail_color;
                uint32_t cape_emblem_color;
                uint32_t cape_shape;
                uint32_t cape_detail;
                uint32_t cape_emblem;
                uint32_t cape_trim;
            };
            const uint32_t Packet<TownAllianceObject>::STATIC_HEADER = 0x11D;

            // Info about any guilds applicable to current outpost.
            // NOTE: When entering a guild hall, that guild will always be the first added (local_id = 1).
            struct GuildGeneral : Packet<GuildGeneral> {
                uint32_t local_id;
                uint32_t ghkey[4]; // blob[16]
                wchar_t name[32];
                wchar_t tag[5];
                uint32_t cape_bg_color;
                uint32_t cape_detail_color;
                uint32_t cape_emblem_color;
                uint32_t cape_shape;
                uint32_t cape_detail;
                uint32_t cape_emblem;
                uint32_t cape_trim;
                uint32_t unk1; // Word
                uint32_t unk2; // byte
                uint32_t unk3; // byte
                uint32_t faction;
                uint32_t unk4; // dword
                uint32_t rank;
                uint32_t allegiance;
                uint32_t unk5; // byte
            };
            const uint32_t Packet<GuildGeneral>::STATIC_HEADER = 0x124;

            // Gold added to inventory
            struct CharacterAddGold : Packet<CharacterAddGold> {
                uint32_t unk;
                uint32_t gold;
            };
            const uint32_t Packet<CharacterAddGold>::STATIC_HEADER = 0x143;

            // Gold removed from inventory
            struct CharacterRemoveGold : Packet<CharacterRemoveGold> {
                uint32_t unk; // some kind of id? but neither agentid nor playerid
                uint32_t gold;
            };
            const uint32_t Packet<CharacterRemoveGold>::STATIC_HEADER = 0x152;

            struct InstanceLoadFile : Packet<InstanceLoadFile> {
                uint32_t map_fileID;
                Vec2f spawn_point;
                uint16_t spawn_plane;
                uint8_t unk1;
                uint8_t unk2;
                uint8_t unk3[8];
            };
            const uint32_t Packet<InstanceLoadFile>::STATIC_HEADER = 0x19A;

            struct InstanceLoadInfo : Packet<InstanceLoadInfo> {
                uint32_t agent_id;
                uint32_t map_id;
                uint32_t is_explorable;
                uint32_t district;
                uint32_t language;
                uint32_t is_observer;
            };
            const uint32_t Packet<InstanceLoadInfo>::STATIC_HEADER = 0x19E;

            struct GameSrvTransfer : Packet<GameSrvTransfer> {
                uint8_t host[24]; // ip of the game server
                uint32_t token1; // world id
                uint32_t region; // uint8_t
                uint32_t map_id; // uint8_t
                uint32_t is_explorable; // uint8_t
                uint32_t token2; // player id
            };
            const uint32_t Packet<GameSrvTransfer>::STATIC_HEADER = 0x1AA;

            struct DoACompleteZone : Packet<DoACompleteZone> {
                wchar_t message[122];
            };
            const uint32_t Packet<DoACompleteZone>::STATIC_HEADER = 0x1B4;

            struct ErrorMessage : Packet<ErrorMessage> {
                uint32_t message_id;
            };
            const uint32_t Packet<ErrorMessage>::STATIC_HEADER = 0x1C1;

            /* Party Invites */
            struct PartyHenchmanAdd : Packet<PartyHenchmanAdd> {
                uint32_t party_id;
                uint32_t agent_id;
                wchar_t name_enc[8];
                uint32_t profession;
                uint32_t level;
            };
            const uint32_t Packet<PartyHenchmanAdd>::STATIC_HEADER = 0x1C4;

            struct PartyHenchmanRemove : Packet<PartyHenchmanRemove> {
                uint32_t party_id;
                uint32_t agent_id;
            };
            const uint32_t Packet<PartyHenchmanRemove>::STATIC_HEADER = 0x1C5;

            struct PartyHeroAdd : Packet<PartyHeroAdd> {
                uint32_t party_id;
                uint32_t owner_player_number;
                uint32_t agent_id;
                uint32_t hero_id;
                uint32_t level;
            };
            const uint32_t Packet<PartyHeroAdd>::STATIC_HEADER = 0x1C7;

            struct PartyHeroRemove : Packet<PartyHeroRemove> {
                uint32_t party_id;
                uint32_t owner_player_number;
                uint32_t agent_id;
            };
            const uint32_t Packet<PartyHeroRemove>::STATIC_HEADER = 0x1C8;


            // Invite sent to another party
            struct PartyInviteSent_Create : Packet<PartyInviteSent_Create> {
                uint32_t target_party_id;
            };
            const uint32_t Packet<PartyInviteSent_Create>::STATIC_HEADER = 0x1C9;
            // Invite received from another party
            struct PartyInviteReceived_Create : Packet<PartyInviteReceived_Create> {
                uint32_t target_party_id; // word
            };
            const uint32_t Packet<PartyInviteReceived_Create>::STATIC_HEADER = 0x1CA;

            // Remove a sent party invite. Invitation has been cancelled.
            struct PartyInviteSent_Cancel : Packet<PartyInviteSent_Cancel> {
                uint32_t target_party_id; // word
            };
            const uint32_t Packet<PartyInviteSent_Cancel>::STATIC_HEADER = 0x1CB;

            // Remove a received party invite. Invitation has been cancelled.
            struct PartyInviteReceived_Cancel : Packet<PartyInviteReceived_Cancel> {
                uint32_t target_party_id; // word
            };
            const uint32_t Packet<PartyInviteReceived_Cancel>::STATIC_HEADER = 0x1CC;

            // Remove a received party invite. Invitation has been either accepted or rejected (we dont know atm)
            struct PartyInviteSent_Response : Packet<PartyInviteSent_Response> {
                uint32_t target_party_id; // word
            };
            const uint32_t Packet<PartyInviteSent_Response>::STATIC_HEADER = 0x1CD;

            // Remove a sent party invite. Invitation has been either accepted or rejected (we dont know atm)
            struct PartyInviteReceived_Response : Packet<PartyInviteReceived_Response> {
                uint32_t target_party_id; // word
            };
            const uint32_t Packet<PartyInviteReceived_Response>::STATIC_HEADER = 0x1CE;

            // A player in a party has been updated or added. Not necessarily yours.
            struct PartyPlayerAdd : Packet<PartyPlayerAdd> {
                uint32_t party_id;
                uint32_t player_id;
                uint32_t state; // 3 = Invited
            };
            const uint32_t Packet<PartyPlayerAdd>::STATIC_HEADER = 0x1D0;
            // Player has left party. Not necessarily yours.
            struct PartyPlayerRemove : Packet<PartyPlayerRemove> {
                uint32_t party_id;
                uint32_t player_id;
            };
            const uint32_t Packet<PartyPlayerRemove>::STATIC_HEADER = 0x1D5;
            // Player in party has toggled ready. Not necessarily yours.
            struct PartyPlayerReady : Packet<PartyPlayerReady> {
                uint32_t party_id;
                uint32_t player_id;
                uint32_t is_ready;
            };
            const uint32_t Packet<PartyPlayerReady>::STATIC_HEADER = 0x1D6;

            // When a new party is created:
            // 1.	PartyPlayerStreamStart packet is sent
            // 2.	PartyPlayerAdd packet per member
            //		PartyHeroAdd packet per hero
            //		PartyHenchmanAdd packer per henchman
            // 3. PartyPlayerStreamEnd packet is sent
            struct PartyPlayerStreamStart : Packet<PartyPlayerStreamStart> {
                uint32_t party_id; // word
            };
            const uint32_t Packet<PartyPlayerStreamStart>::STATIC_HEADER = 0x1D7;
            struct PartyPlayerStreamEnd : Packet<PartyPlayerStreamEnd> {
                uint32_t party_id; // word
            };
            const uint32_t Packet<PartyPlayerStreamEnd>::STATIC_HEADER = 0x1D8;

            struct PartyDefeated : Packet<PartyDefeated> {
            };
            const uint32_t Packet<PartyDefeated>::STATIC_HEADER = 0x1DD;

            struct PartyLock : Packet<PartyLock> { // Sent when party window is locked/unlocked e.g. pending mission entry
                uint32_t unk1; // 2 = locked?
                uint32_t unk2; // 1 = locked?
                wchar_t unk3[8];
            };
            const uint32_t Packet<PartyLock>::STATIC_HEADER = 0x1DE;
        }
    }
}
