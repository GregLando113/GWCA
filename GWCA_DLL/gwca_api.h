#ifdef GWCA_API_H
#error "File included multiple times."
#endif
#define GWCA_API_H


#include "../GWCA/GWStructures.h"

namespace GW {
    
    struct IAgentMgr {
        // Get Agent stuffs
        virtual Agent* GetPlayer() = 0;
        virtual Agent* GetTarget() = 0;
        virtual Agent* GetAgentByID(DWORD id) = 0;
        
        // Get Current AgentID's of player or target.
        virtual DWORD GetPlayerId() = 0;
        virtual DWORD GetTargetId() = 0;
        
        // Returns array of alternate agent array that can be read beyond compass range.
        // Holds limited info and needs to be explored more.
        virtual GW::MapAgentArray GetMapAgentArray() = 0;
        
        // Returns Agentstruct Array of agents in compass range, full structs.
        virtual GW::AgentArray GetAgentArray() = 0;
        
        virtual GW::PlayerArray GetPlayerArray() = 0;
        
        virtual GW::NPCArray GetNPCArray() = 0;
        virtual GW::NPC& GetNPCByID(DWORD id) = 0;
        
        // Computes distance between the two agents in game units
        virtual float GetDistance(Vector2f a, const Vector2f b) = 0;
        
        // Computes squared distance between the two agents in game units
        virtual float GetSqrDistance(Vector2f a, const Vector2f b) = 0;
        
        // Change targeted agent to (Agent)
        virtual void ChangeTarget(GW::Agent* Agent) = 0;
        
        // Move to specified coordinates.
        // Note: will do nothing if coordinate is outside the map!
        virtual void Move(float X, float Y, DWORD ZPlane = 0) = 0;
        
        virtual void Move(const GW::GamePos& pos) = 0;
        
        // Same as pressing button (id) while talking to an NPC.
        virtual void Dialog(DWORD id) = 0;
        
        // Go to an NPC and begin interaction.
        virtual void GoNPC(GW::Agent* Agent, DWORD CallTarget = 0) = 0;
        
        // Walk to a player.
        virtual void GoPlayer(GW::Agent* Agent) = 0;
        
        // Go to a chest/signpost (yellow nametag) specified by (Agent).
        // Also sets agent as your open chest target.
        virtual void GoSignpost(GW::Agent* Agent, BOOL CallTarget = 0) = 0;
        
        // Call target of specified agent without interacting with the agent.
        virtual void CallTarget(GW::Agent* Agent) = 0;
        
        // Returns last dialog id sent to the server.
        virtual DWORD GetLastDialogId() = 0;
        
        // Uses size of player array. Needs testing.
        virtual DWORD GetAmountOfPlayersInInstance() = 0;
        
        // Returns name of player with selected loginnumber.
        virtual wchar_t* GetPlayerNameByLoginNumber(DWORD loginnumber) = 0;
        
        // Returns AgentID of player with selected loginnumber.
        virtual DWORD GetAgentIdByLoginNumber(DWORD loginnumber) = 0;
        
        virtual GW::AgentID GetHeroAgentID(DWORD heroindex) = 0;
        
    };
    
    struct IItemMgr {
        
        // Get full array of items sorted by ItemID.
        virtual GW::ItemArray GetItemArray() = 0;
        
        // Get bag array [1-4] = inventory bags, [8-16] = storage, more in gr wiki.
        virtual GW::Bag** GetBagArray() = 0;
        
        // Use given item if usable.
        virtual void UseItem(GW::Item* item) = 0;
        
        // Find item in selected bags with said modelid, then use it.
        // return: True if found and used, false if not.
        virtual bool UseItemByModelId(DWORD modelid, BYTE bagStart = 1, const BYTE bagEnd = 4) = 0;
        
        // Returns the amount of item with said modelid in given bags.
        virtual DWORD CountItemByModelId(DWORD modelid, BYTE bagStart = 1, const BYTE bagEnd = 4) = 0;
        
        // Returns item struct of item with given modelid.
        virtual GW::Item* GetItemByModelId(DWORD modelid, BYTE bagStart = 1, const BYTE bagEnd = 4) = 0;
        
        // Equip item if equippable.
        virtual void EquipItem(GW::Item* item) = 0;
        
        // Drop item if droppable.
        virtual void DropItem(GW::Item* item, DWORD quantity) = 0;
        
        // Pick up selected item off the ground.
        virtual void PickUpItem(GW::Item* item, DWORD CallTarget = 0) = 0;
        
        // Opens the storage window from anywhere.
        // Can only interact with the storage in an outpost with a xunlai chest inside, sorry no exploiting.
        virtual void OpenXunlaiWindow() = 0;
        
        // Drop amount gold on ground.
        virtual void DropGold(DWORD Amount = 1) = 0;
        
        // Get amount of gold on character.
        virtual DWORD GetGoldAmountOnCharacter() = 0;
        
        // Get amount of gold in storage.
        virtual DWORD GetGoldAmountInStorage() = 0;
        
        // Open locked chest, raw packet, first send a GoSignpost packet to select chest.
        virtual void OpenLockedChest() = 0;
        
    };
    
    struct ISkillbarMgr {
        // Get the skill slot in the player bar of the player.
        // Returns 0 if the skill is not there
        virtual int GetSkillSlot(Constants::SkillID SkillID) = 0;
        
        // Use Skill in slot (Slot) on (Agent), optionally call that you are using said skill.
        virtual void UseSkill(DWORD Slot, DWORD Target = 0, DWORD CallTarget = 0) = 0;
        
        // Send raw packet to use skill with ID (SkillID). 
        // Same as above except the skillbar client struct will not be registered as casting.
        virtual void UseSkillByID(DWORD SkillID, DWORD Target = 0, DWORD CallTarget = 0) = 0;
        
        // Get skill structure of said id, houses pretty much everything you would want to know about the skill.
        virtual GW::Skill GetSkillConstantData(DWORD SkillID) = 0;
        
        virtual void ChangeSecondary(DWORD profession, int heroindex = 0) = 0;
        
        virtual void LoadSkillbar(DWORD* skillids, int heroindex = 0) = 0;
        
        virtual void SetAttributes(DWORD attributecount, DWORD* attributeids, DWORD* attributevalues, int heroindex = 0) = 0;
    };
    
    struct IEffectMgr {
        
        // Get full array of effects and buffs for player and heroes.
        virtual GW::AgentEffectsArray GetPartyEffectArray() = 0;
        
        // Get array of effects on the player.
        virtual GW::EffectArray GetPlayerEffectArray() = 0;
        
        // Get array of buffs on the player.
        virtual GW::BuffArray GetPlayerBuffArray() = 0;
        
        // Drop buffid buff.
        virtual void DropBuff(DWORD buffId) = 0;
        
        // Gets effect struct of effect on player with SkillID, returns Effect::Nil() if no match.
        virtual GW::Effect GetPlayerEffectById(Constants::SkillID SkillID) = 0;
        
        // Gets Buff struct of Buff on player with SkillID, returns Buff::Nil() if no match.
        virtual GW::Buff GetPlayerBuffBySkillId(Constants::SkillID SkillID) = 0;
        
        // Returns current level of intoxication, 0-5 scale.
        // If > 0 then skills that benefit from drunk will work.
        virtual DWORD GetAlcoholLevel() = 0;
        
        // Have fun with this ;))))))))))
        virtual void GetDrunkAf(DWORD Intensity, DWORD Tint) = 0;
        
    };
    
    struct IGuildMgr {
        
        // Array of guilds, holds basically everything about a guild. Can get structs of all players in outpost ;)
        virtual GW::GuildArray GetGuildArray() = 0;
        
        // Index in guild array of player guild.
        virtual DWORD GetPlayerGuildIndex() = 0;
        
        // Announcement in guild at the moment.
        virtual wchar_t* GetPlayerGuildAnnouncement() = 0;
        
        // Name of player who last edited the announcement.
        virtual wchar_t* GetPlayerGuildAnnouncer() = 0;
        
        // Travels to players Guild Hall
        virtual void TravelGH() = 0;
        
        // Travels to Guild Hall with specified Guild GUID
        virtual void TravelGH(GW::GHKey key) = 0;
        
        virtual void LeaveGH() = 0;
        
    };
    
    struct IMapMgr {
        virtual bool IsMapLoaded() = 0;
        
        // Get current map ID.
        virtual GW::Constants::MapID GetMapID() = 0;
        
        // Get current region you are in.
        virtual int GetRegion() = 0;
        
        // Get current language you are in.
        virtual int GetLanguage() = 0;
        
        // Get time, in ms, since the instance you are residing in has been created.
        virtual DWORD GetInstanceTime() = 0;
        
        // Get the instance type (Outpost, Explorable or Loading)
        virtual Constants::InstanceType GetInstanceType() = 0;
        
        // Travel to specified outpost.
        virtual void Travel(Constants::MapID MapID, int District = 0, int Region = 0, int Language = 0) = 0;
        virtual void Travel(Constants::MapID MapID, Constants::District district, int district_number = 0) = 0;
        
        // Returns array of icons (res shrines, quarries, traders, etc) on mission map.
        // Look at MissionMapIcon struct for more info.
        virtual GW::MissionMapIconArray GetMissionMapIconArray() = 0;
        
        // Returns pointer of collision trapezoid array.
        virtual GW::PathingMapArray GetPathingMap() = 0;
    };
    
    struct ICtoSMgr {
        
        
        // send packet via a buffer
        void SendPacket(DWORD len,void* packet) = 0;
    };
    
    struct IStoCMgr {
        
    };
    
    struct ICameraMgr {
        // X,Y,Z of camera in game world.
        virtual Vector3f GetCameraPosition() = 0;
        
        // LookAt Target (players head if not modified) of camera.
        virtual Vector3f GetLookAtTarget() = 0;
        
        // camera yaw in radians from east
        virtual float GetYaw() = 0;
        
        // camera pitch
        virtual float GetPitch() = 0;
        
        // Distance of camera from lookat target (player)
        virtual float GetCameraZoom() = 0;
        
        // Horizonal Field of View
        virtual float GetFieldOfView() = 0;
        
        // Returns (possible?) projection matrix of the game. Needs to be delved into.
        virtual float* GetProjectionMatrix() = 0;
        
        // Set yaw, radian angle
        virtual void SetYaw(float yaw) = 0;
        
        // Set pitch (sin(angle))
        virtual void SetPitch(float pitch) = 0;
        
        // Manual computation of the position of the Camera. (As close as possible to the original)
        virtual void UpdateCameraPos() = 0;
        virtual Vector3f ComputeCamPos(float dist = 0) = 0; // 2.f is the first person dist (const by gw)
        virtual void SetCameraPos(Vector3f const& newPos) = 0;
        
        // Change max zoom dist
        virtual void SetMaxDist(float dist) = 0;
        
        // Unlock camera & return the new state of it
        virtual bool UnlockCam(bool flag) = 0;
        virtual bool GetCameraUnlock() = 0;
        
        virtual void SetLookAtTarget(Vector3f const& newPos) = 0;
        
        virtual void ForwardMovement(float amount) = 0;
        virtual void SideMovement(float amount) = 0;
        virtual void VerticalMovement(float amount) = 0;
        
        // Enable or Disable the fog & return the state of it
        virtual bool SetFog(bool flag) = 0;
        
        virtual void SetFieldOfView(float fov) = 0;
    };
    
    struct IMerchantMgr {
        virtual GW::MerchItemArray GetMerchantItemsArray() = 0;
        
        virtual GW::Item* GetMerchantItemByModelID(DWORD modelid);
        virtual void EnqueueTransaction(
            TransactionType type,
            DWORD gold_give = 0,
            DWORD itemcount_give = 0,
            DWORD* itemids_give = 0,
            BYTE* itemquantities_give = 0,
            DWORD gold_receive = 0,
            DWORD itemcount_receive = 0,
            DWORD* itemids_receive = 0,
            BYTE* itemquantities_receive = 0
            ) = 0;
        
        virtual void EnqueueQuoteRequest(
            TransactionType type,
            DWORD itemcount_give = 0,
            DWORD* itemids_give = 0,
            BYTE* itemquantities_give = 0,
            DWORD itemcount_receive = 0,
            DWORD* itemids_receive = 0,
            BYTE* itemquantities_receive = 0
            ) = 0;
        
        virtual void BuyMerchantItem(DWORD modelid, DWORD quantity = 1) = 0;
        
        virtual void SellMerchantItem(GW::Item* itemtosell, DWORD sellquantity = NULL) = 0;
    };
    
    struct IFriendListMgr {
        
        virtual GW::Friend* getFriend(DWORD index) = 0;
        virtual DWORD size() = 0;
        virtual DWORD getNumberOfFriends() = 0;
        virtual DWORD getNumberOfIgnores() = 0;
        virtual DWORD getNumberOfPartners() = 0;
        virtual DWORD getNumberOfTraders() = 0;
        
        virtual DWORD getMyStatus() = 0;
        
        virtual void SetFriendListStatus(Constants::OnlineStatus status) = 0;
        
    };
    
    struct IChatMgr {
        
        typedef DWORD Color;
        typedef DWORD Channel;
        
        // Send a message to an in-game channel (! for all, @ for guild, etc)
        virtual void SendChat(const wchar_t* msg, wchar_t channel) = 0;
        
        // Write to chat as a PM with printf style arguments.
        virtual void WriteChatF(const wchar_t* from, const wchar_t* format, ...) = 0;
        
        // Simple write to chat as a PM
        virtual void WriteChat(const wchar_t* from, const wchar_t* msg) = 0;
        
        virtual Color SetSenderColor(Channel chan, Color col) = 0;
        virtual Color SetMessageColor(Channel chan, Color col) = 0;
    };
    
    struct IApi {
        
        virtual IAgentMgr* Agents() = 0;
        virtual IItemMgr* Items() = 0;
        virtual ISkillbarMgr* Skillbars() = 0;
        virtual IGuildMgr* Guilds() = 0;
        virtual IMapMgr* Maps() = 0;
        virtual ICtoSMgr* CtoS() = 0;
        virtual IStoCMgr* StoC() = 0;
        virtual ICameraMgr* Camera() = 0;
        virtual IMerchantMgr* Merchant() = 0;
        virtual IFriendListMgr* FriendList() = 0;
        virtual IChatMgr* Chat() = 0;
    };
    
    typedef IApi* QueryMain_t();
    
}

#if 0
// Just here as an example
GW::IApi* GetGWCA(const char* dllname = "GWApi.dll") {
    HMODULE hGWCA = GetModuleHandleA(dllname);
    
    if(!hGWCA)
        return nullptr;
    
    GW::QueryMain_t* fnGetMain = reinterpret_cast<GW::QueryMain_t*>(GetProcAddress(hGWCA,"QueryInterface"));
    
    if(!fnGetMain)
        return nullptr;
    
    return fnGetMain();
}
    #endif