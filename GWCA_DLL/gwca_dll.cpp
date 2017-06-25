#include <Windows.h>

#include "GWCA_API.h"

#include <GWCA\GWCA.h>
#include <GWCA\Managers\AgentMgr.h>
#include <GWCA\Managers\ItemMgr.h>
#include <GWCA\Managers\SkillbarMgr.h>
#include <GWCA\Managers\GuildMgr.h>
#include <GWCA\Managers\MapMgr.h>
#include <GWCA\Managers\EffectMgr.h>
#include <GWCA\Managers\ChatMgr.h>
#include <GWCA\Managers\FriendListMgr.h>
#include <GWCA\Managers\MerchantMgr.h>
#include <GWCA\Managers\CameraMgr.h>

#pragma comment(lib, "GWCAd.lib")

using namespace GW;

Agent* GetPlayer() {
    return Agents::GetPlayer();
}
Agent* GetTarget() {
    return Agents::GetTarget();
}
Agent* GetAgentByID(DWORD id) {
    return Agents::GetAgentByID(id);
}

// Get Current AgentID's of player or target.
DWORD GetPlayerId() {
    return Agents::GetPlayerId();
}
DWORD GetTargetId() {
    return Agents::GetTargetId();
}

// Returns array of alternate agent array that can be read beyond compass range.
// Holds limited info and needs to be explored more.
GW::MapAgentArray GetMapAgentArray() {
    return Agents::GetMapAgentArray();
}

// Returns Agentstruct Array of agents in compass range, full structs.
GW::AgentArray GetAgentArray() {
    return Agents::GetAgentArray();
}

GW::PlayerArray GetPlayerArray() {
    return Agents::GetPlayerArray();
}

GW::NPCArray GetNPCArray() {
    return Agents::GetNPCArray();
}
GW::NPC& GetNPCByID(DWORD id) {
    return Agents::GetNPCByID(id);
}

// Computes distance between the two agents in game units
float GetDistance(Vector2f a, const Vector2f b) {
    return Agents::GetDistance(a,b);
}

// Computes squared distance between the two agents in game units
float GetSqrDistance(Vector2f a, const Vector2f b) {
    return Agents::GetSqrDistance(a,b);
}

// Change targeted agent to (Agent)
void ChangeTarget(GW::Agent* agent) {
    return Agents::ChangeTarget(agent);
}

// Move to specified coordinates.
// Note: will do nothing if coordinate is outside the map!
void Move(float X, float Y, DWORD ZPlane = 0) {
    return Agents::Move(X,Y,ZPlane);
}

void Move(const GW::GamePos& pos) {
    return Agents::Move(pos);
}

// Same as pressing button (id) while talking to an NPC.
void Dialog(DWORD id) {
    return Agents::Dialog(id);
}

// Go to an NPC and begin interaction.
void GoNPC(GW::Agent* Agent, DWORD CallTarget = 0) {
    return Agents::GoNPC(Agent,CallTarget);
}

// Walk to a player.
void GoPlayer(GW::Agent* Agent) {
    return Agents::GoPlayer(Agent);
}

// Go to a chest/signpost (yellow nametag) specified by (Agent).
// Also sets agent as your open chest target.
void GoSignpost(GW::Agent* Agent, BOOL CallTarget = 0) {
    return Agents::GoSignpost(Agent,CallTarget);
}

// Call target of specified agent without interacting with the agent.
void CallTarget(GW::Agent* Agent) {
    return Agents::CallTarget(Agent);
}

// Returns last dialog id sent to the server.
DWORD GetLastDialogId() {
    return Agents::GetLastDialogId();
}

// Uses size of player array. Needs testing.
DWORD GetAmountOfPlayersInInstance() {
    return Agents::GetAmountOfPlayersInInstance();
}

// Returns name of player with selected loginnumber.
wchar_t* GetPlayerNameByLoginNumber(DWORD loginnumber) {
    return Agents::GetPlayerNameByLoginNumber(loginnumber);
}

// Returns AgentID of player with selected loginnumber.
DWORD GetAgentIdByLoginNumber(DWORD loginnumber) {
    return Agents::GetAgentIdByLoginNumber(loginnumber);
}

GW::AgentID GetHeroAgentID(DWORD heroindex) {
    return Agents::GetHeroAgentID(heroindex);
}

// Get full array of items sorted by ItemID.
GW::ItemArray GetItemArray() {
    return Items::GetItemArray();
}

// Get bag array [1-4] = inventory bags, [8-16] = storage, more in gr wiki.
GW::Bag** GetBagArray() {
    return Items::GetBagArray();
}

// Use given item if usable.
void UseItem(GW::Item* item) {
    return Items::UseItem(item);
}

// Find item in selected bags with said modelid, then use it.
// return: True if found and used, false if not.
bool UseItemByModelId(DWORD modelid, BYTE bagStart = 1, const BYTE bagEnd = 4){
    return Items::UseItemByModelId(modelid,bagStart,bagEnd);
}

// Returns the amount of item with said modelid in given bags.
DWORD CountItemByModelId(DWORD modelid, BYTE bagStart = 1, const BYTE bagEnd = 4){
    //return CountItemByModelId(modelid,bagStart,bagEnd);
	return 0;
}

// Returns item struct of item with given modelid.
GW::Item* GetItemByModelId(DWORD modelid, BYTE bagStart = 1, const BYTE bagEnd = 4){
    return Items::GetItemByModelId(modelid,bagStart,bagEnd);
}

// Equip item if equippable.
void EquipItem(GW::Item* item){
    return Items::EquipItem(item);
}

// Drop item if droppable.
void DropItem(GW::Item* item, DWORD quantity){
    return Items::DropItem(item,quantity);
}

// Pick up selected item off the ground.
void PickUpItem(GW::Item* item, DWORD CallTarget = 0){
    return Items::PickUpItem(item,CallTarget);
}

// Opens the storage window from anywhere.
// Can only interact with the storage in an outpost with a xunlai chest inside, sorry no exploiting.
void OpenXunlaiWindow(){
    return Items::OpenXunlaiWindow();
}

// Drop amount gold on ground.
void DropGold(DWORD Amount){
    return Items::DropGold(Amount);
}

// Get amount of gold on character.
DWORD GetGoldAmountOnCharacter(){
    return Items::GetGoldAmountOnCharacter();
}

// Get amount of gold in storage.
DWORD GetGoldAmountInStorage(){
    return Items::GetGoldAmountInStorage();
}

// Open locked chest, raw packet, first send a GoSignpost packet to select chest.
void OpenLockedChest(){
    return Items::OpenLockedChest();
}

// Get the skill slot in the player bar of the player.
// Returns 0 if the skill is not there
int GetSkillSlot(Constants::SkillID SkillID) {
    return SkillbarMgr::GetSkillSlot(SkillID);
}

// Use Skill in slot (Slot) on (Agent), optionally call that you are using said skill.
void UseSkill(DWORD Slot, DWORD Target = 0, DWORD CallTarget = 0) {
    return SkillbarMgr::UseSkill(Slot,Target,CallTarget);
}

// Send raw packet to use skill with ID (SkillID). 
// Same as above except the skillbar client struct will not be registered as casting.
void UseSkillByID(DWORD SkillID, DWORD Target = 0, DWORD CallTarget = 0) {
    return SkillbarMgr::UseSkillByID(SkillID,Target,CallTarget);
}

// Get skill structure of said id, houses pretty much everything you would want to know about the skill.
GW::Skill GetSkillConstantData(DWORD SkillID) {
    return SkillbarMgr::GetSkillConstantData(SkillID);
}

void ChangeSecondary(DWORD profession, int heroindex = 0) {
    return SkillbarMgr::ChangeSecondary(profession,heroindex);
}

void LoadSkillbar(DWORD* skillids, int heroindex = 0) {
    return SkillbarMgr::LoadSkillbar(skillids,heroindex);
}

void SetAttributes(DWORD attributecount, DWORD* attributeids, DWORD* attributevalues, int heroindex = 0) {
    return SkillbarMgr::SetAttributes(attributecount,attributeids, attributevalues,heroindex);
}

// Get full array of effects and buffs for player and heroes.
GW::AgentEffectsArray GetPartyEffectArray() {
    return Effects::GetPartyEffectArray();
}

// Get array of effects on the player.
GW::EffectArray GetPlayerEffectArray() {
    return Effects::GetPlayerEffectArray();
}

// Get array of buffs on the player.
GW::BuffArray GetPlayerBuffArray() {
    return Effects::GetPlayerBuffArray();
}

// Drop buffid buff.
void DropBuff(DWORD buffId) {
    return Effects::DropBuff(buffId);
}

// Gets effect struct of effect on player with SkillID, returns Effect::Nil() if no match.
GW::Effect GetPlayerEffectById(Constants::SkillID SkillID) {
    return Effects::GetPlayerEffectById(SkillID);
}

// Gets Buff struct of Buff on player with SkillID, returns Buff::Nil() if no match.
GW::Buff GetPlayerBuffBySkillId(Constants::SkillID SkillID) {
    return Effects::GetPlayerBuffBySkillId(SkillID);
}

// Returns current level of intoxication, 0-5 scale.
// If > 0 then skills that benefit from drunk will work.
DWORD GetAlcoholLevel() {
    return Effects::GetAlcoholLevel();
}

// Have fun with this ;))))))))))
void GetDrunkAf(DWORD Intensity, DWORD Tint) {
    return Effects::GetDrunkAf(Intensity,Tint);
}

// Array of guilds, holds basically everything about a guild. Can get structs of all players in outpost ;)
GW::GuildArray GetGuildArray() {
    return GuildMgr::GetGuildArray();
}

// Index in guild array of player guild.
DWORD GetPlayerGuildIndex() {
    return GuildMgr::GetPlayerGuildIndex();
}

// Announcement in guild at the moment.
wchar_t* GetPlayerGuildAnnouncement() {
    return GuildMgr::GetPlayerGuildAnnouncement();
}

// Name of player who last edited the announcement.
wchar_t* GetPlayerGuildAnnouncer() {
    return GuildMgr::GetPlayerGuildAnnouncer();
}

// Travels to players Guild Hall
void TravelGH() {
    return GuildMgr::TravelGH();
}

// Travels to Guild Hall with specified Guild GUID
void TravelGH(GW::GHKey key) {
    return GuildMgr::TravelGH(key);
}

void LeaveGH() {
    return GuildMgr::LeaveGH();
}

bool IsMapLoaded() {
    return Map::IsMapLoaded();
}

// Get current map ID.
Constants::MapID GetMapID() {
    return Map::GetMapID();
}

// Get current region you are in.
int GetRegion() {
    return Map::GetRegion();
}

// Get current language you are in.
int GetLanguage() {
    return Map::GetLanguage();
}

// Get time, in ms, since the instance you are residing in has been created.
DWORD GetInstanceTime() {
    return Map::GetInstanceTime();
}

// Get the instance type (Outpost, Explorable or Loading)
Constants::InstanceType GetInstanceType() {
    return Map::GetInstanceType();
}

// Travel to specified outpost.
void Travel(Constants::MapID MapID, int District = 0, int Region = 0, int Language = 0) {
    return Map::Travel(MapID,District,Region,Language);
}
void Travel(Constants::MapID MapID, Constants::District district, int district_number = 0) {
    return Map::Travel(MapID,district,district_number);
}

// Returns array of icons (res shrines, quarries, traders, etc) on mission map.
// Look at MissionMapIcon struct for more info.
GW::MissionMapIconArray GetMissionMapIconArray() {
    return Map::GetMissionMapIconArray();
}

// Returns pointer of collision trapezoid array.
GW::PathingMapArray GetPathingMap() {
    return Map::GetPathingMap();
}

// send packet via a buffer
void SendPacket(DWORD len,void* packet);

// X,Y,Z of camera in game world.
GW::Vector3f GetCameraPosition() {
    return CameraMgr::GetCameraPosition();
}

// LookAt Target (players head if not modified) of camera.
GW::Vector3f GetLookAtTarget() {
    return CameraMgr::GetLookAtTarget();
}

// camera yaw in radians from east
float GetYaw() {
    return CameraMgr::GetYaw();
}

// camera pitch
float GetPitch() {
    return CameraMgr::GetPitch();
}

// Distance of camera from lookat target (player)
float GetCameraZoom() {
    return CameraMgr::GetCameraZoom();
}

// Horizonal Field of View
float GetFieldOfView() {
    return CameraMgr::GetFieldOfView();
}

// Returns (possible?) projection matrix of the game. Needs to be delved into.
float* GetProjectionMatrix() {
    return CameraMgr::GetProjectionMatrix();
}

// Set yaw, radian angle
void SetYaw(float yaw) {
    return CameraMgr::SetYaw(yaw);
}

// Set pitch (sin(angle))
void SetPitch(float pitch) {
    return CameraMgr::SetPitch(pitch);
}

// Manual computation of the position of the Camera. (As close as possible to the original)
void UpdateCameraPos() {
    return CameraMgr::UpdateCameraPos();
}
GW::Vector3f ComputeCamPos(float dist = 0)  {
    return CameraMgr::ComputeCamPos(dist);
} 
// 2.f is the first person dist (const by gw)
void SetCameraPos(GW::Vector3f const& newPos) {
    return CameraMgr::SetCameraPos(newPos);
}

// Change max zoom dist
void SetMaxDist(float dist) {
    return CameraMgr::SetMaxDist(dist);
}

// Unlock camera & return the new state of it
bool UnlockCam(bool flag) {
    return CameraMgr::UnlockCam(flag);
}
bool GetCameraUnlock() {
    return CameraMgr::GetCameraUnlock();
}

void SetLookAtTarget(GW::Vector3f const& newPos) {
    return CameraMgr::SetLookAtTarget(newPos);
}

void ForwardMovement(float amount) {
    return CameraMgr::ForwardMovement(amount);
}
void SideMovement(float amount) {
    return CameraMgr::SideMovement(amount);
}
void VerticalMovement(float amount) {
    return CameraMgr::VerticalMovement(amount);
}

// Enable or Disable the fog & return the state of it
bool SetFog(bool flag) {
    return CameraMgr::SetFog(flag);
}

void SetFieldOfView(float fov) {
    return CameraMgr::SetFieldOfView(fov);
}

GW::MerchItemArray GetMerchantItemsArray();

// TODO(Greg): make dese
GW::Item* GetMerchantItemByModelID(DWORD modelid);

void BuyMerchantItem(DWORD modelid, DWORD quantity) {
    // return Merchant::BuyMerchantItem(modileid,quantity);
}

void SellMerchantItem(GW::Item* itemtosell, DWORD sellquantity = NULL) {
    // return Merchant::SellMerchantItem(itemtosellmodileid,sellquantity);
}

GW::Friend* getFriend(DWORD index) {
    return FriendListMgr::GetFriend(index);
}

DWORD getNumberOfFriends(){
    return FriendListMgr::GetNumberOfFriends();
}
DWORD getNumberOfIgnores(){
    return FriendListMgr::GetNumberOfIgnores();
}
DWORD getNumberOfPartners(){
    return FriendListMgr::GetNumberOfPartners();
}
DWORD getNumberOfTraders(){
    return FriendListMgr::GetNumberOfTraders();
}

DWORD getMyStatus(){
    return FriendListMgr::GetMyStatus();
}

void SetFriendListStatus(Constants::OnlineStatus status){
    return FriendListMgr::SetFriendListStatus(status);
}

// Send a message to an in-game channel (! for all, @ for guild, etc)
void SendChat(const wchar_t* msg, wchar_t channel) {
    return Chat::SendChat(msg,channel);
}

// Write to chat as a PM with printf style arguments.
void WriteChatF(const wchar_t* from, const wchar_t* format, ...) {
    
}

// Simple write to chat as a PM
void WriteChat(const wchar_t* from, const wchar_t* msg) {
    return Chat::WriteChat(from,msg);
}

Chat::Color SetSenderColor(Chat::Channel chan, Chat::Color col) {
    return Chat::SetSenderColor(chan,col);
}
Chat::Color SetMessageColor(Chat::Channel chan, Chat::Color col) {
    return Chat::SetMessageColor(chan,col);
}

DWORD WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved) {
    if (dwReason == DLL_PROCESS_ATTACH) {
        if(!GW::Initialize())
            return FALSE;
    }
    else if (dwReason == DLL_PROCESS_DETACH) {
        GW::Terminate();
    }
    return TRUE;
}