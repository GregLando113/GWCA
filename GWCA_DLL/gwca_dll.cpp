

#include <Windows.h>


#include "GWCA_API.h"

#include "../GWCA/GWCA.h"

using namespace GW;

struct AgentMgr_Impl : public IAgentMgr {
    
    Agent* GetPlayer() {
        return Agents().GetPlayer();
    }
    Agent* GetTarget() {
        return Agents().GetTarget();
    }
    Agent* GetAgentByID(DWORD id) {
        return Agents().GetAgentByID(id);
    }
    
    // Get Current AgentID's of player or target.
    DWORD GetPlayerId() {
        return Agents().GetPlayerId();
    }
    DWORD GetTargetId() {
        return Agents().GetTargetId();
    }
    
    // Returns array of alternate agent array that can be read beyond compass range.
    // Holds limited info and needs to be explored more.
    GW::MapAgentArray GetMapAgentArray() {
        return Agents().GetMapAgentArray();
    }
    
    // Returns Agentstruct Array of agents in compass range, full structs.
    GW::AgentArray GetAgentArray() {
        return Agents().GetAgentArray();
    }
    
    GW::PlayerArray GetPlayerArray() {
        return Agents().GetPlayerArray();
    }
    
    GW::NPCArray GetNPCArray() {
        return Agents().GetNPCArray();
    }
    GW::NPC& GetNPCByID(DWORD id) {
        return Agents().GetNPCByID(id);
    }
    
    // Computes distance between the two agents in game units
    float GetDistance(Vector2f a, const Vector2f b) {
        return Agents().GetDistance(a,b);
    }
    
    // Computes squared distance between the two agents in game units
    float GetSqrDistance(Vector2f a, const Vector2f b) {
        return Agents().GetSqrDistance(a,b);
    }
    
    // Change targeted agent to (Agent)
    void ChangeTarget(GW::Agent* agent) {
        return Agents().ChangeTarget(agent);
    }
    
    // Move to specified coordinates.
    // Note: will do nothing if coordinate is outside the map!
    void Move(float X, float Y, DWORD ZPlane = 0) {
        return Agents().Move(X,Y,ZPlane);
    }
    
    void Move(const GW::GamePos& pos) {
        return Agents().Move(pos);
    }
    
    // Same as pressing button (id) while talking to an NPC.
    void Dialog(DWORD id) {
        return Agents().Dialog(id);
    }
    
    // Go to an NPC and begin interaction.
    void GoNPC(GW::Agent* Agent, DWORD CallTarget = 0) {
        return Agents().GoNPC(Agent,CallTarget);
    }
    
    // Walk to a player.
    void GoPlayer(GW::Agent* Agent) {
        return Agents().GoPlayer(Agent);
    }
    
    // Go to a chest/signpost (yellow nametag) specified by (Agent).
    // Also sets agent as your open chest target.
    void GoSignpost(GW::Agent* Agent, BOOL CallTarget = 0) {
        return Agents().GoSignpost(Agent,CallTarget);
    }
    
    // Call target of specified agent without interacting with the agent.
    void CallTarget(GW::Agent* Agent) {
        return Agents().CallTarget(Agent);
    }
    
    // Returns last dialog id sent to the server.
    DWORD GetLastDialogId() {
        return Agents().GetLastDialogId();
    }
    
    // Uses size of player array. Needs testing.
    DWORD GetAmountOfPlayersInInstance() {
        return Agents().GetAmountOfPlayersInInstance();
    }
    
    // Returns name of player with selected loginnumber.
    wchar_t* GetPlayerNameByLoginNumber(DWORD loginnumber) {
        return Agents().GetPlayerNameByLoginNumber(loginnumber);
    }
    
    // Returns AgentID of player with selected loginnumber.
    DWORD GetAgentIdByLoginNumber(DWORD loginnumber) {
        return Agents().GetAgentIdByLoginNumber(loginnumber);
    }
    
    GW::AgentID GetHeroAgentID(DWORD heroindex) {
        return Agents().GetHeroAgentID(heroindex);
    }
    
};


struct ItemMgr_Impl : public IItemMgr {
    
    // Get full array of items sorted by ItemID.
    GW::ItemArray GetItemArray() {
        return Items().GetItemArray();
    }
    
    // Get bag array [1-4] = inventory bags, [8-16] = storage, more in gr wiki.
    GW::Bag** GetBagArray() {
        return Items().GetBagArray();
    }
    
    // Use given item if usable.
    void UseItem(GW::Item* item) {
        return Items().UseItem(item);
    }
    
    // Find item in selected bags with said modelid, then use it.
    // return: True if found and used, false if not.
    bool UseItemByModelId(DWORD modelid, BYTE bagStart = 1, const BYTE bagEnd = 4){
        return Items().UseItemByModelId(modelid,bagStart,bagEnd);
    }
    
    // Returns the amount of item with said modelid in given bags.
    DWORD CountItemByModelId(DWORD modelid, BYTE bagStart = 1, const BYTE bagEnd = 4){
        return CountItemByModelId(modelid,bagStart,bagEnd);
    }
    
    // Returns item struct of item with given modelid.
    GW::Item* GetItemByModelId(DWORD modelid, BYTE bagStart = 1, const BYTE bagEnd = 4){
        return Items().GetItemByModelId(modelid,bagStart,bagEnd);
    }
    
    // Equip item if equippable.
    void EquipItem(GW::Item* item){
        return Items().EquipItem(item);
    }
    
    // Drop item if droppable.
    void DropItem(GW::Item* item, DWORD quantity){
        return Items().DropItem(item,quantity);
    }
    
    // Pick up selected item off the ground.
    void PickUpItem(GW::Item* item, DWORD CallTarget = 0){
        return Items().PickUpItem(item,CallTarget);
    }
    
    // Opens the storage window from anywhere.
    // Can only interact with the storage in an outpost with a xunlai chest inside, sorry no exploiting.
    void OpenXunlaiWindow(){
        return Items().OpenXunlaiWindow();
    }
    
    // Drop amount gold on ground.
    void DropGold(DWORD Amount = 1){
        return Items().DropGold(Amount);
    }
    
    // Get amount of gold on character.
    DWORD GetGoldAmountOnCharacter(){
        return Items().GetGoldAmountOnCharacter();
    }
    
    // Get amount of gold in storage.
    DWORD GetGoldAmountInStorage(){
        return Items().GetGoldAmountInStorage();
    }
    
    // Open locked chest, raw packet, first send a GoSignpost packet to select chest.
    void OpenLockedChest(){
        return Items().OpenLockedChest();
    }
    
};



struct SkillbarMgr_Impl : public ISkillbarMgr {
    // Get the skill slot in the player bar of the player.
    // Returns 0 if the skill is not there
    int GetSkillSlot(Constants::SkillID SkillID) {
        return Skillbarmgr().GetSkillSlot(SkillID);
    }
    
    // Use Skill in slot (Slot) on (Agent), optionally call that you are using said skill.
    void UseSkill(DWORD Slot, DWORD Target = 0, DWORD CallTarget = 0) {
        return Skillbarmgr().UseSkill(Slot,Target,CallTarget);
    }
    
    // Send raw packet to use skill with ID (SkillID). 
    // Same as above except the skillbar client struct will not be registered as casting.
    void UseSkillByID(DWORD SkillID, DWORD Target = 0, DWORD CallTarget = 0) {
        return Skillbarmgr().UseSkillByID(SkillID,Target,CallTarget);
    }
    
    // Get skill structure of said id, houses pretty much everything you would want to know about the skill.
    GW::Skill GetSkillConstantData(DWORD SkillID) {
        return Skillbarmgr().GetSkillConstantData(SkillID);
    }
    
    void ChangeSecondary(DWORD profession, int heroindex = 0) {
        return Skillbarmgr().ChangeSecondary(profession,heroindex);
    }
    
    void LoadSkillbar(DWORD* skillids, int heroindex = 0) {
        return Skillbarmgr().LoadSkillbar(skillids,heroindex);
    }
    
    void SetAttributes(DWORD attributecount, DWORD* attributeids, DWORD* attributevalues, int heroindex = 0) {
        return Skillbarmgr().SetAttributes(attributecount,attributeids, attributevalues,heroindex);
    }
};


struct EffectMgr_Impl : public IEffectMgr {
    
    // Get full array of effects and buffs for player and heroes.
    GW::AgentEffectsArray GetPartyEffectArray() {
        return Effects().GetPartyEffectArray();
    }
    
    // Get array of effects on the player.
    GW::EffectArray GetPlayerEffectArray() {
        return Effects().GetPlayerEffectArray();
    }
    
    // Get array of buffs on the player.
    GW::BuffArray GetPlayerBuffArray() {
        return Effects().GetPlayerBuffArray();
    }
    
    // Drop buffid buff.
    void DropBuff(DWORD buffId) {
        return Effects().DropBuff(buffId);
    }
    
    // Gets effect struct of effect on player with SkillID, returns Effect::Nil() if no match.
    GW::Effect GetPlayerEffectById(Constants::SkillID SkillID) {
        return Effects().GetPlayerEffectById(SkillID);
    }
    
    // Gets Buff struct of Buff on player with SkillID, returns Buff::Nil() if no match.
    GW::Buff GetPlayerBuffBySkillId(Constants::SkillID SkillID) {
        return Effects().GetPlayerBuffBySkillId(SkillID);
    }
    
    // Returns current level of intoxication, 0-5 scale.
    // If > 0 then skills that benefit from drunk will work.
    DWORD GetAlcoholLevel() {
        return Effects().GetAlcoholLevel();
    }
    
    // Have fun with this ;))))))))))
    void GetDrunkAf(DWORD Intensity, DWORD Tint) {
        return Effects().GetDrunkAf(Intensity,Tint);
    }
    
};

struct GuildMgr_Impl : public IGuildMgr {
    
    // Array of guilds, holds basically everything about a guild. Can get structs of all players in outpost ;)
    GW::GuildArray GetGuildArray() {
        return Guildmgr().GetGuildArray();
    }
    
    // Index in guild array of player guild.
    DWORD GetPlayerGuildIndex() {
        return Guildmgr().GetPlayerGuildIndex();
    }
    
    // Announcement in guild at the moment.
    wchar_t* GetPlayerGuildAnnouncement() {
        return Guildmgr().GetPlayerGuildAnnouncement();
    }
    
    // Name of player who last edited the announcement.
    wchar_t* GetPlayerGuildAnnouncer() {
        return Guildmgr().GetPlayerGuildAnnouncer();
    }
    
    // Travels to players Guild Hall
    void TravelGH() {
        return Guildmgr().TravelGH();
    }
    
    // Travels to Guild Hall with specified Guild GUID
    void TravelGH(GW::GHKey key) {
        return Guildmgr().TravelGH(key);
    }
    
    void LeaveGH() {
        return Guildmgr().LeaveGH();
    }
    
};


struct MapMgr_Impl : public IMapMgr {
    bool IsMapLoaded() {
        return Map().IsMapLoaded();
    }
    
    // Get current map ID.
    GW::Constants::MapID GetMapID() {
        return Map().GetMapID();
    }
    
    // Get current region you are in.
    int GetRegion() {
        return Map().GetRegion();
    }
    
    // Get current language you are in.
    int GetLanguage() {
        return Map().GetLanguage();
    }
    
    // Get time, in ms, since the instance you are residing in has been created.
    DWORD GetInstanceTime() {
        return Map().GetInstanceTime();
    }
    
    // Get the instance type (Outpost, Explorable or Loading)
    Constants::InstanceType GetInstanceType() {
        return Map().GetInstanceType();
    }
    
    // Travel to specified outpost.
    void Travel(Constants::MapID MapID, int District = 0, int Region = 0, int Language = 0) {
        return Map().Travel(MapID,District,Region,Language);
    }
    void Travel(Constants::MapID MapID, Constants::District district, int district_number = 0) {
        return Map().Travel(MapID,district,district_number);
    }
    
    // Returns array of icons (res shrines, quarries, traders, etc) on mission map.
    // Look at MissionMapIcon struct for more info.
    GW::MissionMapIconArray GetMissionMapIconArray() {
        return Map().GetMissionMapIconArray();
    }
    
    // Returns pointer of collision trapezoid array.
    GW::PathingMapArray GetPathingMap() {
        return Map().GetPathingMap();
    }
};


struct CtoSMgr_Impl : public ICtoSMgr {
    // send packet via a buffer
    void SendPacket(DWORD len,void* packet) = 0;
};

struct StoCMgr_Impl : public IStoCMgr {
    
};

struct CameraMgr_Impl : public ICameraMgr {
    // X,Y,Z of camera in game world.
    Vector3f GetCameraPosition() {
        return Camera().GetCameraPosition();
    }
    
    // LookAt Target (players head if not modified) of camera.
    Vector3f GetLookAtTarget() {
        return Camera().GetLookAtTarget();
    }
    
    // camera yaw in radians from east
    float GetYaw() {
        return Camera().GetYaw();
    }
    
    // camera pitch
    float GetPitch() {
        return Camera().GetPitch();
    }
    
    // Distance of camera from lookat target (player)
    float GetCameraZoom() {
        return Camera().GetCameraZoom();
    }
    
    // Horizonal Field of View
    float GetFieldOfView() {
        return Camera().GetFieldOfView();
    }
    
    // Returns (possible?) projection matrix of the game. Needs to be delved into.
    float* GetProjectionMatrix() {
        return Camera().GetProjectionMatrix();
    }
    
    // Set yaw, radian angle
    void SetYaw(float yaw) {
        return Camera().SetYaw(yaw);
    }
    
    // Set pitch (sin(angle))
    void SetPitch(float pitch) {
        return Camera().SetPitch(pitch);
    }
    
    // Manual computation of the position of the Camera. (As close as possible to the original)
    void UpdateCameraPos() {
        return Camera().UpdateCameraPos();
    }
    Vector3f ComputeCamPos(float dist = 0)  {
        return Camera().ComputeCamPos(dist);
    } 
    // 2.f is the first person dist (const by gw)
    void SetCameraPos(Vector3f const& newPos) {
        return Camera().SetCameraPos(newPos);
    }
    
    // Change max zoom dist
    void SetMaxDist(float dist) {
        return Camera().SetMaxDist(dist);
    }
    
    // Unlock camera & return the new state of it
    bool UnlockCam(bool flag) {
        return Camera().UnlockCam(flag);
    }
    bool GetCameraUnlock() {
        return Camera().GetCameraUnlock();
    }
    
    void SetLookAtTarget(Vector3f const& newPos) {
        return Camera().SetLookAtTarget(newPos);
    }
    
    void ForwardMovement(float amount) {
        return Camera().ForwardMovement(amount);
    }
    void SideMovement(float amount) {
        return Camera().SideMovement(amount);
    }
    void VerticalMovement(float amount) {
        return Camera().VerticalMovement(amount);
    }
    
    // Enable or Disable the fog & return the state of it
    bool SetFog(bool flag) {
        return Camera().SetFog(flag);
    }
    
    void SetFieldOfView(float fov) {
        return Camera().SetFieldOfView(fov);
    }
};

struct MerchantMgr_Impl : public IMerchantMgr {
    GW::MerchItemArray GetMerchantItemsArray();
    
    // TODO(Greg): make dese
    GW::Item* GetMerchantItemByModelID(DWORD modelid);
    void EnqueueTransaction(
        TransactionType type,
        DWORD gold_give = 0,
        DWORD itemcount_give = 0,
        DWORD* itemids_give = 0,
        BYTE* itemquantities_give = 0,
        DWORD gold_receive = 0,
        DWORD itemcount_receive = 0,
        DWORD* itemids_receive = 0,
        BYTE* itemquantities_receive = 0
        ){}
    
    
    void EnqueueQuoteRequest(
        TransactionType type,
        DWORD itemcount_give = 0,
        DWORD* itemids_give = 0,
        BYTE* itemquantities_give = 0,
        DWORD itemcount_receive = 0,
        DWORD* itemids_receive = 0,
        BYTE* itemquantities_receive = 0
        ){}
    
    void BuyMerchantItem(DWORD modelid, DWORD quantity = 1) {
        return Merchant().BuyMerchantItem(modileid,quantity);
    }
    
    void SellMerchantItem(GW::Item* itemtosell, DWORD sellquantity = NULL) {
        return Merchant().SellMerchantItem(itemtosellmodileid,sellquantity);
    }
};

struct FriendListMgr_Impl : public IFriendListMgr {
    
    virtual GW::Friend* getFriend(DWORD index) {
        return FriendListmgr().getFriend(index);
    }
    
    virtual DWORD size(){
        return FriendListmgr().size();
    }
    virtual DWORD getNumberOfFriends(){
        return FriendListmgr().getNumberOfFriends();
    }
    virtual DWORD getNumberOfIgnores(){
        return FriendListmgr().getNumberOfIgnores();
    }
    virtual DWORD getNumberOfPartners(){
        return FriendListmgr().getNumberOfPartners();
    }
    virtual DWORD getNumberOfTraders(){
        return FriendListmgr().getNumberOfTraders();
    }
    
    virtual DWORD getMyStatus(){
        return FriendListmgr().getMyStatus();
    }
    
    virtual void SetFriendListStatus(Constants::OnlineStatus status){
        return FriendListmgr().SetFriendListStatus(status);
    }
    
};

struct ChatMgr_Impl : public IChatMgr {
    
    typedef DWORD Color;
    typedef DWORD Channel;
    
    // Send a message to an in-game channel (! for all, @ for guild, etc)
    virtual void SendChat(const wchar_t* msg, wchar_t channel) {
        return Chat().SendChat(msg,channel);
    }
    
    // Write to chat as a PM with printf style arguments.
    virtual void WriteChatF(const wchar_t* from, const wchar_t* format, ...) {
        
    }
    
    // Simple write to chat as a PM
    virtual void WriteChat(const wchar_t* from, const wchar_t* msg) {
        return Chat().WriteChat(from,msg);
    }
    
    virtual Color SetSenderColor(Channel chan, Color col) {
        return Chat().SetSenderColor(chan,col);
    }
    virtual Color SetMessageColor(Channel chan, Color col) {
        return Chat().SetMessageColor(chan,col);
    }
};

struct Api_Impl : IApi {
    
    IAgentMgr* Agents() { return &_Agents; }
    IItemMgr* Items() { return &_Items; }
    ISkillbarMgr* Skillbars() { return &_Skillbars; }
    IGuildMgr* Guilds() { return &_Guilds; }
    IMapMgr* Maps() { return &_Maps; }
    ICtoSMgr* CtoS() { return &_CtoS; }
    IStoCMgr* StoC() { return &_StoC; }
    ICameraMgr* Camera() { return &_Camera; }
    IMerchantMgr* Merchant() { return &_Merchant; }
    IFriendListMgr* FriendList() { return &_FriendList; }
    IChatMgr* Chat()  { return &_Chat; }
    
    AgentMgr_Impl _Agents;
    ItemMgr_Impl _Items;
    SkillbarMgr_Impl _Skillbars;
    GuildMgr_Impl _Guilds;
    MapMgr_Impl _Maps;
    CtoSMgr_Impl _CtoS;
    StoCMgr_Impl _StoC;
    CameraMgr_Impl _Camera;
    MerchantMgr_Impl _Merchant;
    FriendListMgr_Impl _FriendList;
    ChatMgr_Impl _Chat;
};

Api_Impl g__mainapi;

extern "C"
__declspec(dllexport)
IApi* 
QueryInterface() { return &g__mainapi; }

DWORD WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved) {
    
    switch (dwReason) {
        
        case DLL_PROCESS_ATTACH: {
            if(!GW::Api::Initialize())
                return FALSE;
        }break;
        
        case DLL_PROCESS_DETACH: {
            GW::Api::Destruct();
        }break;
        
    }
    
    return TRUE;
}