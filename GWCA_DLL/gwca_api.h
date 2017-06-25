#ifndef GWCA_API_H
#define GWCA_API_H

#include <Windows.h>
#include <stdint.h>
#include <string>

// @TODO, fix thoses includes.
#include <GWCA\GWStructures.h>
#include <GWCA\Constants\Constants.h>
#include <GWCA\Constants\Skills.h>
#include <GWCA\Constants\Maps.h>

#define GWCA_MAJOR (36604)
#define GWCA_MINOR (0)

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

    using CString = std::wstring;

    struct Vec2f { float x, y; };
    struct Vec3f { float x, y, z; };

    enum Event {
        EVENT_TICK,

        EVENT_PACKET_RECEIVED,

        EVENT_MESSAGE_RECEIVED,

        EVENT_COUNT
    };

    GWAPI void RegisterEvent  (HMODULE plugin, Event e);
    GWAPI void UnregisterEvent(HMODULE plugin, Event e);

    GWAPI HMODULE LoadPlugin(CString path);
    GWAPI void    UnloadPlugin(HMODULE plugin);

    GWAPI CString GetPluginsDir();
    GWAPI DWORD   GetGlobalTime();

    //////////////////////////////
    //          Agent
    //////////////////////////////

    // Get GW::Agent stuffs
    GWAPI GW::Agent* GetPlayer();
    GWAPI GW::Agent* GetTarget();
    GWAPI GW::Agent* GetAgentByID(DWORD id);
    
    // Get Current GW::AgentID's of player or target.
    GWAPI DWORD GetPlayerId();
    GWAPI DWORD GetTargetId();
    
    // Returns array of alternate GW::Agent array that can be read beyond compass range.
    // Holds limited info and needs to be explored more.
    GWAPI GW::MapAgentArray GetMapAgentArray();
    
    // Returns GW::Agentstruct Array of GW::Agents in compass range, full structs.
    GWAPI GW::AgentArray GetAgentArray();
    
    GWAPI GW::PlayerArray GetPlayerArray();
    
    GWAPI GW::NPCArray GetNPCArray();
    GWAPI GW::NPC& GetNPCByID(DWORD id);
    
    // Computes distance between the two GW::Agents in game units
    GWAPI float GetDistance(Vec2f a, Vec2f b);
    
    // Computes squared distance between the two GW::Agents in game units
    GWAPI float GetSqrDistance(Vec2f a, const Vec2f b);
    
    // Change targeted GW::Agent to (GW::Agent)
    GWAPI void ChangeTarget(GW::Agent* Agent);
    
    // Move to specified coordinates.
    // Note: will do nothing if coordinate is outside the map!
    GWAPI void Move(float x, float y, DWORD ZPlane);
    GWAPI void Move(Vec2f pos);
    
    // Same as pressing button (id) while talking to an NPC.
    GWAPI void Dialog(DWORD id);
    
    // Go to an NPC and begin interaction.
    GWAPI void GoNPC(GW::Agent* Agent, DWORD CallTarget);
    
    // Walk to a player.
    GWAPI void GoPlayer(GW::Agent* Agent);
    
    // Go to a chest/signpost (yellow nametag) specified by (GW::Agent).
    // Also sets GW::Agent as your open chest target.
    GWAPI void GoSignpost(GW::Agent* Agent, BOOL CallTarget);
    
    // Call target of specified GW::Agent without interacting with the GW::Agent.
    GWAPI void CallTarget(GW::Agent* Agent);
    
    // Returns last dialog id sent to the server.
    GWAPI DWORD GetLastDialogId();
    
    // Uses size of player array. Needs testing.
    GWAPI DWORD GetAmountOfPlayersInInstance();
    
    // Returns name of player with selected loginnumber.
    GWAPI wchar* GetPlayerNameByLoginNumber(DWORD loginnumber);
    
    // Returns GW::AgentID of player with selected loginnumber.
    GWAPI DWORD GetAgentIdByLoginNumber(DWORD loginnumber);
    
    GWAPI GW::AgentID GetHeroAgentID(DWORD heroindex);
        
    //////////////////////////////
    //          Item
    //////////////////////////////
    
    // Get full array of items sorted by ItemID.
    GWAPI GW::ItemArray GetItemArray();
    
    // Get bag array [1-4] = inventory bags, [8-16] = storage, more in gr wiki.
    GWAPI GW::Bag** GetBagArray();
    
    // Use given item if usable.
    GWAPI void UseItem(GW::Item* item);
    
    // Find item in selected bags with said modelid, then use it.
    // return: True if found and used, false if not.
    GWAPI bool UseItemByModelId(DWORD modelid, BYTE bagStart = 1, BYTE bagEnd = 4);
    
    // Returns the amount of item with said modelid in given bags.
    GWAPI DWORD CountItemByModelId(DWORD modelid, BYTE bagStart = 1, BYTE bagEnd = 4);
    
    // Returns item struct of item with given modelid.
    GWAPI GW::Item* GetItemByModelId(DWORD modelid, BYTE bagStart = 1, BYTE bagEnd = 4);
    
    // Equip item if equippable.
    GWAPI void EquipItem(GW::Item* item);
    
    // Drop item if droppable.
    GWAPI void DropItem(GW::Item* item, DWORD quantity);
    
    // Pick up selected item off the ground.
    GWAPI void PickUpItem(GW::Item* item, DWORD CallTarget);
    
    // Opens the storage window from anywhere.
    // Can only interact with the storage in an outpost with a xunlai chest inside, sorry no exploiting.
    GWAPI void OpenXunlaiWindow();
    
    // Drop amount gold on ground.
    GWAPI void DropGold(DWORD Amount = 1);
    
    // Get amount of gold on character.
    GWAPI DWORD GetGoldAmountOnCharacter();
    
    // Get amount of gold in storage.
    GWAPI DWORD GetGoldAmountInStorage();
    
    // Open locked chest, raw packet, first send a GoSignpost packet to select chest.
    GWAPI void OpenLockedChest();
        
    //////////////////////////////
    //        Skillbar
    //////////////////////////////
    
    // Get the skill slot in the player bar of the player.
    // Returns 0 if the skill is not there
    GWAPI int GetSkillSlot(GW::Constants::SkillID SkillID);
    
    // Use Skill in slot (Slot) on (GW::Agent), optionally call that you are using said skill.
    GWAPI void UseSkill(DWORD Slot, DWORD Target, DWORD CallTarget);
    
    // Send raw packet to use skill with ID (SkillID). 
    // Same as above except the skillbar client struct will not be registered as casting.
    GWAPI void UseSkillByID(DWORD SkillID, DWORD Target, DWORD CallTarget);
    
    // Get skill structure of said id, houses pretty much everything you would want to know about the skill.
    GWAPI GW::Skill GetSkillConstantData(DWORD SkillID);
    
    GWAPI void ChangeSecondary(DWORD profession, int heroindex);
    
    GWAPI void LoadSkillbar(DWORD* skillids, int heroindex);
    
    GWAPI void SetAttributes(DWORD attributecount, DWORD* attributeids, DWORD* attributevalues, int heroindex);
    
    //////////////////////////////
    //        Effects
    //////////////////////////////
    // Get full array of effects and buffs for player and heroes.
    GWAPI GW::AgentEffectsArray GetPartyEffectArray();
    
    // Get array of effects on the player.
    GWAPI GW::EffectArray GetPlayerEffectArray();
    
    // Get array of buffs on the player.
    GWAPI GW::BuffArray GetPlayerBuffArray();
    
    // Drop buffid buff.
    GWAPI void DropBuff(DWORD buffId);
    
    // Gets effect struct of effect on player with SkillID, returns Effect::Nil() if no match.
    GWAPI GW::Effect GetPlayerEffectById(GW::Constants::SkillID SkillID);
    
    // Gets Buff struct of Buff on player with SkillID, returns Buff::Nil() if no match.
    GWAPI GW::Buff GetPlayerBuffBySkillId(GW::Constants::SkillID SkillID);
    
    // Returns current level of intoxication, 0-5 scale.
    // If > 0 then skills that benefit from drunk will work.
    GWAPI DWORD GetAlcoholLevel();
    
    // Have fun with this ;))))))))))
    GWAPI void GetDrunkAf(DWORD Intensity, DWORD Tint);
    
    //////////////////////////////
    //           Guild
    //////////////////////////////
    // Array of guilds, holds basically everything about a guild. Can get structs of all players in outpost ;)
    GWAPI GW::GuildArray GetGuildArray();
    
    // Index in guild array of player guild.
    GWAPI DWORD GetPlayerGuildIndex();
    
    // Announcement in guild at the moment.
    GWAPI wchar* GetPlayerGuildAnnouncement();
    
    // Name of player who last edited the announcement.
    GWAPI wchar* GetPlayerGuildAnnouncer();
    
    // Travels to players Guild Hall
    GWAPI void TravelGH();
    
    // Travels to Guild Hall with specified Guild GUID
    GWAPI void TravelGH(GW::GHKey key);
    
    GWAPI void LeaveGH();
    
    //////////////////////////////
    //           Map
    //////////////////////////////
    GWAPI bool IsMapLoaded();
    
    // Get current map ID.
    GWAPI GW::Constants::MapID GetMapID();
    
    // Get current region you are in.
    GWAPI int GetRegion();
    
    // Get current language you are in.
    GWAPI int GetLanguage();
    
    // Get time, in ms, since the instance you are residing in has been created.
    GWAPI DWORD GetInstanceTime();
    
    // Get the instance type (Outpost, Explorable or Loading)
    GWAPI GW::Constants::InstanceType GetInstanceType();
    
    // Travel to specified outpost.
    GWAPI void Travel(GW::Constants::MapID MapID, int District, int Region, int Language);
    GWAPI void Travel(GW::Constants::MapID MapID, GW::Constants::District district, int district_number);
    
    // Returns array of icons (res shrines, quarries, traders, etc) on mission map.
    // Look at MissionMapIcon struct for more info.
    GWAPI GW::MissionMapIconArray GetMissionMapIconArray();
    
    // Returns pointer of collision trapezoid array.
    GWAPI GW::PathingMapArray GetPathingMap();
    
    //////////////////////////////
    //           CtoS
    //////////////////////////////
    // send packet via a buffer
    // template <typename T>
    // GWAPI void SendPacket(Packet<T> *p);

    //////////////////////////////
    //         Camera
    //////////////////////////////

    // X,Y,Z of camera in game world.
	GWAPI Vec3f GetCameraPosition();

    // LookAt Target (players head if not modified) of camera.
    GWAPI Vec3f GetLookAtTarget();

    GWAPI void FlipCameraYaw(float degree); 
    
    // camera yaw in radians from east
    GWAPI float GetYaw();
    
    // camera pitch
    GWAPI float GetPitch();
    
    // Distance of camera from lookat target (player)
    GWAPI float GetCameraZoom();
    
    // Horizonal Field of View
    GWAPI float GetFieldOfView();
    
    // Returns (possible?) projection matrix of the game. Needs to be delved into.
    GWAPI float* GetProjectionMatrix();
    
    // Set yaw, radian angle
    GWAPI void SetYaw(float yaw);
    
    // Set pitch (sin(angle))
    GWAPI void SetPitch(float pitch);
    
    // Manual computation of the position of the Camera. (As close as possible to the original)
    GWAPI void UpdateCameraPos();
    GWAPI Vec3f ComputeCamPos(float dist); // 2.f is the first person dist (const by gw)
    GWAPI void SetCameraPos(Vec3f newPos);
    
    // Change max zoom dist
    GWAPI void SetMaxDist(float dist);
    
    // Unlock camera & return the new state of it
    GWAPI bool UnlockCam(bool flag);
    GWAPI bool GetCameraUnlock();
    
    GWAPI void SetLookAtTarget(Vec3f newPos);
    
    GWAPI void ForwardMovement(float amount);
    GWAPI void SideMovement(float amount);
    GWAPI void VerticalMovement(float amount);
    
    // Enable or Disable the fog & return the state of it
    GWAPI bool SetFog(bool flag);
    
    GWAPI void SetFieldOfView(float fov);

    //////////////////////////////
    //         Merchant
    //////////////////////////////
    GWAPI GW::MerchItemArray GetMerchantItemsArray();
    
    GWAPI GW::Item* GetMerchantItemByModelID(DWORD modelid);
    
    GWAPI void BuyMerchantItem(DWORD modelid, DWORD quantity = 1);
    
    GWAPI void SellMerchantItem(GW::Item* itemtosell, DWORD sellquantity = NULL);
    
    //////////////////////////////
    //        FriendList
    //////////////////////////////
    GWAPI GW::Friend* GetFriend(CString name);
    GWAPI DWORD GetFriendsCount();
    GWAPI DWORD GetIgnoresCount();
    
    GWAPI DWORD GetPlayerStatus();
    
    // void SetFriendListStatus(Constants::OnlineStatus status);

    //////////////////////////////
    //           Chat
    //////////////////////////////
    // Send a message to an in-game channel (! for all, @ for guild, etc)
    GWAPI void SendChat(const wchar *msg, wchar channel);
    
    // Write to chat as a PM with printf style arguments.
    GWAPI void WriteChatF(const wchar *from, const wchar *format, ...);
    
    // Simple write to chat as a PM
    GWAPI void WriteChat(const wchar *from, const wchar *msg);
    
	enum Channel; // @Remove
    GWAPI Color SetSenderColor(Channel chan, Color col);
    GWAPI Color SetMessageColor(Channel chan, Color col);
} // namespace GW

#endif // GWCA_API_H
