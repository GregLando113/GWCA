#ifndef GWCA_API_H
#define GWCA_API_H

#include <GWCA\GWStructures.h>
#include <GWCA\Constants\Constants.h>
#include <GWCA\Constants\Skills.h>
#include <GWCA\Constants\Maps.h>

namespace GW {
    //////////////////////////////
    //          Agent
    //////////////////////////////

    // Get Agent stuffs
    Agent* GetPlayer();
    Agent* GetTarget();
    Agent* GetAgentByID(DWORD id);
    
    // Get Current AgentID's of player or target.
    DWORD GetPlayerId();
    DWORD GetTargetId();
    
    // Returns array of alternate agent array that can be read beyond compass range.
    // Holds limited info and needs to be explored more.
    GW::MapAgentArray GetMapAgentArray();
    
    // Returns Agentstruct Array of agents in compass range, full structs.
    GW::AgentArray GetAgentArray();
    
    GW::PlayerArray GetPlayerArray();
    
    GW::NPCArray GetNPCArray();
    GW::NPC& GetNPCByID(DWORD id);
    
    // Computes distance between the two agents in game units
    float GetDistance(Vector2f a, const Vector2f b);
    
    // Computes squared distance between the two agents in game units
    float GetSqrDistance(Vector2f a, const Vector2f b);
    
    // Change targeted agent to (Agent)
    void ChangeTarget(GW::Agent* Agent);
    
    // Move to specified coordinates.
    // Note: will do nothing if coordinate is outside the map!
    void Move(float X, float Y, DWORD ZPlane);
    
    void Move(const GW::GamePos& pos);
    
    // Same as pressing button (id) while talking to an NPC.
    void Dialog(DWORD id);
    
    // Go to an NPC and begin interaction.
    void GoNPC(GW::Agent* Agent, DWORD CallTarget);
    
    // Walk to a player.
    void GoPlayer(GW::Agent* Agent);
    
    // Go to a chest/signpost (yellow nametag) specified by (Agent).
    // Also sets agent as your open chest target.
    void GoSignpost(GW::Agent* Agent, BOOL CallTarget);
    
    // Call target of specified agent without interacting with the agent.
    void CallTarget(GW::Agent* Agent);
    
    // Returns last dialog id sent to the server.
    DWORD GetLastDialogId();
    
    // Uses size of player array. Needs testing.
    DWORD GetAmountOfPlayersInInstance();
    
    // Returns name of player with selected loginnumber.
    wchar_t* GetPlayerNameByLoginNumber(DWORD loginnumber);
    
    // Returns AgentID of player with selected loginnumber.
    DWORD GetAgentIdByLoginNumber(DWORD loginnumber);
    
    GW::AgentID GetHeroAgentID(DWORD heroindex);
        
    //////////////////////////////
    //          Item
    //////////////////////////////
    
    // Get full array of items sorted by ItemID.
    GW::ItemArray GetItemArray();
    
    // Get bag array [1-4] = inventory bags, [8-16] = storage, more in gr wiki.
    GW::Bag** GetBagArray();
    
    // Use given item if usable.
    void UseItem(GW::Item* item);
    
    // Find item in selected bags with said modelid, then use it.
    // return: True if found and used, false if not.
    bool UseItemByModelId(DWORD modelid, BYTE bagStart = 1, const BYTE bagEnd = 4);
    
    // Returns the amount of item with said modelid in given bags.
    DWORD CountItemByModelId(DWORD modelid, BYTE bagStart = 1, const BYTE bagEnd = 4);
    
    // Returns item struct of item with given modelid.
    GW::Item* GetItemByModelId(DWORD modelid, BYTE bagStart = 1, const BYTE bagEnd = 4);
    
    // Equip item if equippable.
    void EquipItem(GW::Item* item);
    
    // Drop item if droppable.
    void DropItem(GW::Item* item, DWORD quantity);
    
    // Pick up selected item off the ground.
    void PickUpItem(GW::Item* item, DWORD CallTarget);
    
    // Opens the storage window from anywhere.
    // Can only interact with the storage in an outpost with a xunlai chest inside, sorry no exploiting.
    void OpenXunlaiWindow();
    
    // Drop amount gold on ground.
    void DropGold(DWORD Amount = 1);
    
    // Get amount of gold on character.
    DWORD GetGoldAmountOnCharacter();
    
    // Get amount of gold in storage.
    DWORD GetGoldAmountInStorage();
    
    // Open locked chest, raw packet, first send a GoSignpost packet to select chest.
    void OpenLockedChest();
        
    //////////////////////////////
    //        Skillbar
    //////////////////////////////
    
    // Get the skill slot in the player bar of the player.
    // Returns 0 if the skill is not there
    int GetSkillSlot(Constants::SkillID SkillID);
    
    // Use Skill in slot (Slot) on (Agent), optionally call that you are using said skill.
    void UseSkill(DWORD Slot, DWORD Target, DWORD CallTarget);
    
    // Send raw packet to use skill with ID (SkillID). 
    // Same as above except the skillbar client struct will not be registered as casting.
    void UseSkillByID(DWORD SkillID, DWORD Target, DWORD CallTarget);
    
    // Get skill structure of said id, houses pretty much everything you would want to know about the skill.
    GW::Skill GetSkillConstantData(DWORD SkillID);
    
    void ChangeSecondary(DWORD profession, int heroindex);
    
    void LoadSkillbar(DWORD* skillids, int heroindex);
    
    void SetAttributes(DWORD attributecount, DWORD* attributeids, DWORD* attributevalues, int heroindex);
    
    //////////////////////////////
    //        Effects
    //////////////////////////////
    // Get full array of effects and buffs for player and heroes.
    GW::AgentEffectsArray GetPartyEffectArray();
    
    // Get array of effects on the player.
    GW::EffectArray GetPlayerEffectArray();
    
    // Get array of buffs on the player.
    GW::BuffArray GetPlayerBuffArray();
    
    // Drop buffid buff.
    void DropBuff(DWORD buffId);
    
    // Gets effect struct of effect on player with SkillID, returns Effect::Nil() if no match.
    GW::Effect GetPlayerEffectById(GW::Constants::SkillID SkillID);
    
    // Gets Buff struct of Buff on player with SkillID, returns Buff::Nil() if no match.
    GW::Buff GetPlayerBuffBySkillId(GW::Constants::SkillID SkillID);
    
    // Returns current level of intoxication, 0-5 scale.
    // If > 0 then skills that benefit from drunk will work.
    DWORD GetAlcoholLevel();
    
    // Have fun with this ;))))))))))
    void GetDrunkAf(DWORD Intensity, DWORD Tint);
    
    //////////////////////////////
    //           Guild
    //////////////////////////////
    // Array of guilds, holds basically everything about a guild. Can get structs of all players in outpost ;)
    GW::GuildArray GetGuildArray();
    
    // Index in guild array of player guild.
    DWORD GetPlayerGuildIndex();
    
    // Announcement in guild at the moment.
    wchar_t* GetPlayerGuildAnnouncement();
    
    // Name of player who last edited the announcement.
    wchar_t* GetPlayerGuildAnnouncer();
    
    // Travels to players Guild Hall
    void TravelGH();
    
    // Travels to Guild Hall with specified Guild GUID
    void TravelGH(GW::GHKey key);
    
    void LeaveGH();
    
    //////////////////////////////
    //           Map
    //////////////////////////////
    bool IsMapLoaded();
    
    // Get current map ID.
    GW::Constants::MapID GetMapID();
    
    // Get current region you are in.
    int GetRegion();
    
    // Get current language you are in.
    int GetLanguage();
    
    // Get time, in ms, since the instance you are residing in has been created.
    DWORD GetInstanceTime();
    
    // Get the instance type (Outpost, Explorable or Loading)
    Constants::InstanceType GetInstanceType();
    
    // Travel to specified outpost.
    void Travel(GW::Constants::MapID MapID, int District, int Region, int Language);
    void Travel(GW::Constants::MapID MapID, Constants::District district, int district_number);
    
    // Returns array of icons (res shrines, quarries, traders, etc) on mission map.
    // Look at MissionMapIcon struct for more info.
    GW::MissionMapIconArray GetMissionMapIconArray();
    
    // Returns pointer of collision trapezoid array.
    GW::PathingMapArray GetPathingMap();
    
    //////////////////////////////
    //           CtoS
    //////////////////////////////
    // send packet via a buffer
    void SendPacket(DWORD len, void* packet);

    //////////////////////////////
    //         Camera
    //////////////////////////////
    // X,Y,Z of camera in game world.
	GW::Vector3f GetCameraPosition();
    
    // LookAt Target (players head if not modified) of camera.
    GW::Vector3f GetLookAtTarget();
    
    // camera yaw in radians from east
    float GetYaw();
    
    // camera pitch
    float GetPitch();
    
    // Distance of camera from lookat target (player)
    float GetCameraZoom();
    
    // Horizonal Field of View
    float GetFieldOfView();
    
    // Returns (possible?) projection matrix of the game. Needs to be delved into.
    float* GetProjectionMatrix();
    
    // Set yaw, radian angle
    void SetYaw(float yaw);
    
    // Set pitch (sin(angle))
    void SetPitch(float pitch);
    
    // Manual computation of the position of the Camera. (As close as possible to the original)
    void UpdateCameraPos();
    Vector3f ComputeCamPos(float dist); // 2.f is the first person dist (const by gw)
    void SetCameraPos(Vector3f const& newPos);
    
    // Change max zoom dist
    void SetMaxDist(float dist);
    
    // Unlock camera & return the new state of it
    bool UnlockCam(bool flag);
    bool GetCameraUnlock();
    
    void SetLookAtTarget(Vector3f const& newPos);
    
    void ForwardMovement(float amount);
    void SideMovement(float amount);
    void VerticalMovement(float amount);
    
    // Enable or Disable the fog & return the state of it
    bool SetFog(bool flag);
    
    void SetFieldOfView(float fov);

    //////////////////////////////
    //         Merchant
    //////////////////////////////
    GW::MerchItemArray GetMerchantItemsArray();
    
    GW::Item* GetMerchantItemByModelID(DWORD modelid);
    
    void BuyMerchantItem(DWORD modelid, DWORD quantity = 1);
    
    void SellMerchantItem(GW::Item* itemtosell, DWORD sellquantity = NULL);
    
    //////////////////////////////
    //        FriendList
    //////////////////////////////
    GW::Friend* getFriend(DWORD index);
    DWORD getNumberOfFriends();
    DWORD getNumberOfIgnores();
    DWORD getNumberOfPartners();
    DWORD getNumberOfTraders();
    
    DWORD getMyStatus();
    
    // void SetFriendListStatus(Constants::OnlineStatus status);

    //////////////////////////////
    //           Chat
    //////////////////////////////
    typedef DWORD Color;
    typedef DWORD Channel;

    // Send a message to an in-game channel (! for all, @ for guild, etc)
    void SendChat(const wchar_t* msg, wchar_t channel);
    
    // Write to chat as a PM with printf style arguments.
    void WriteChatF(const wchar_t* from, const wchar_t* format, ...);
    
    // Simple write to chat as a PM
    void WriteChat(const wchar_t* from, const wchar_t* msg);
    
    Color SetSenderColor(Channel chan, Color col);
    Color SetMessageColor(Channel chan, Color col);
} // namespace GW
#endif // GWCA_API_H
