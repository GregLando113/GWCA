#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Windowsx.h>

#include <string>
#include <vector>
#include <functional>

#include <d3dx9tex.h>

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_dx9.h"

#include <GWCA/GWCA.h>
#include <GWCA/Utilities/Hooker.h>

#include <GWCA/Constants/Constants.h>

#include <GWCA/GameEntities/Map.h>

#include <GWCA/Context/GameContext.h>
#include <GWCA/Context/WorldContext.h>

#include <GWCA/Managers/UIMgr.h>
#include <GWCA/Managers/MapMgr.h>
#include <GWCA/Managers/ChatMgr.h>
#include <GWCA/Managers/MemoryMgr.h>
#include <GWCA/Managers/RenderMgr.h>

struct IDirect3DDevice9;

static volatile bool running;
static long          OldWndProc;

static std::vector<std::wstring> map_names;
static GW::Constants::MapID missions[] = {
    GW::Constants::MapID::The_Great_Northern_Wall,
    GW::Constants::MapID::Fort_Ranik,
    GW::Constants::MapID::Ruins_of_Surmia,
    GW::Constants::MapID::Nolani_Academy,
    GW::Constants::MapID::Borlis_Pass,
    GW::Constants::MapID::The_Frost_Gate,
    GW::Constants::MapID::Gates_of_Kryta,
    GW::Constants::MapID::DAlessio_Seaboard,
    GW::Constants::MapID::Divinity_Coast,
    GW::Constants::MapID::The_Wilds,
    GW::Constants::MapID::Bloodstone_Fen,
    GW::Constants::MapID::Aurora_Glade,
    GW::Constants::MapID::Riverside_Province,
    GW::Constants::MapID::Sanctum_Cay,
    GW::Constants::MapID::Dunes_of_Despair,
    GW::Constants::MapID::Thirsty_River,
    GW::Constants::MapID::Elona_Reach,
    GW::Constants::MapID::Augury_Rock_mission,
    GW::Constants::MapID::The_Dragons_Lair,
    GW::Constants::MapID::Ice_Caves_of_Sorrow,
    GW::Constants::MapID::Iron_Mines_of_Moladune,
    GW::Constants::MapID::Thunderhead_Keep,
    GW::Constants::MapID::Ring_of_Fire,
    GW::Constants::MapID::Abaddons_Mouth,
    GW::Constants::MapID::Hells_Precipice,

    GW::Constants::MapID::Minister_Chos_Estate_outpost_mission,
    GW::Constants::MapID::Zen_Daijun_outpost_mission,
    GW::Constants::MapID::Vizunah_Square_Local_Quarter_outpost,
    GW::Constants::MapID::Vizunah_Square_Foreign_Quarter_outpost,
    GW::Constants::MapID::Nahpui_Quarter_outpost_mission,
    GW::Constants::MapID::Tahnnakai_Temple_outpost_mission,
    GW::Constants::MapID::Arborstone_outpost_mission,
    GW::Constants::MapID::Boreas_Seabed_outpost_mission,
    GW::Constants::MapID::Sunjiang_District_outpost_mission,
    GW::Constants::MapID::The_Eternal_Grove_outpost_mission,
    GW::Constants::MapID::Unwaking_Waters_Kurzick_outpost,
    GW::Constants::MapID::Gyala_Hatchery_outpost_mission,
    GW::Constants::MapID::Unwaking_Waters_Luxon_outpost,
    GW::Constants::MapID::Raisu_Palace_outpost_mission,
    GW::Constants::MapID::Imperial_Sanctum_outpost_mission,

    GW::Constants::MapID::Chahbek_Village,
    GW::Constants::MapID::Jokanur_Diggings,
    GW::Constants::MapID::Blacktide_Den,
    GW::Constants::MapID::Consulate_Docks,
    GW::Constants::MapID::Venta_Cemetery,
    GW::Constants::MapID::Kodonur_Crossroads,
    GW::Constants::MapID::Pogahn_Passage,
    GW::Constants::MapID::Rilohn_Refuge,
    GW::Constants::MapID::Moddok_Crevice,
    GW::Constants::MapID::Tihark_Orchard,
    GW::Constants::MapID::Dasha_Vestibule,
    GW::Constants::MapID::Dzagonur_Bastion,
    GW::Constants::MapID::Grand_Court_of_Sebelkeh,
    GW::Constants::MapID::Jennurs_Horde,
    GW::Constants::MapID::Nundu_Bay,
    GW::Constants::MapID::Gate_of_Desolation,
    GW::Constants::MapID::Ruins_of_Morah,
    GW::Constants::MapID::Gate_of_Pain,
    GW::Constants::MapID::Gate_of_Madness,
    GW::Constants::MapID::Abaddons_Gate,
};
static uint32_t n_missions = sizeof(missions) / sizeof(missions[0]);

static LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    static bool right_mouse_down = false;

    if (Message == WM_RBUTTONDOWN) right_mouse_down = true;
    if (Message == WM_RBUTTONDBLCLK) right_mouse_down = true;
    if (Message == WM_RBUTTONUP) right_mouse_down = false;

    ImGuiIO& io = ImGui::GetIO();
    switch (Message) {
        case WM_LBUTTONDOWN:
        case WM_LBUTTONDBLCLK:
            if (!right_mouse_down) io.MouseDown[0] = true;
            break;
        case WM_LBUTTONUP:
            io.MouseDown[0] = false;
            break;
        case WM_MBUTTONDOWN:
        case WM_MBUTTONDBLCLK:
            if (!right_mouse_down) {
                io.KeysDown[VK_MBUTTON] = true;
                io.MouseDown[2] = true;
            }
            break;
        case WM_MBUTTONUP:
            io.KeysDown[VK_MBUTTON] = false;
            io.MouseDown[2] = false;
            break;
        case WM_MOUSEWHEEL:
            if (!right_mouse_down) io.MouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +1.0f : -1.0f;
            break;
        case WM_MOUSEMOVE:
            if (!right_mouse_down) {
                io.MousePos.x = (float)GET_X_LPARAM(lParam);
                io.MousePos.y = (float)GET_Y_LPARAM(lParam);
            }
            break;
        case WM_XBUTTONDOWN:
            if (!right_mouse_down) {
                if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) io.KeysDown[VK_XBUTTON1] = true;
                if (GET_XBUTTON_WPARAM(wParam) == XBUTTON2) io.KeysDown[VK_XBUTTON2] = true;
            }
            break;
        case WM_XBUTTONUP:
            if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) io.KeysDown[VK_XBUTTON1] = false;
            if (GET_XBUTTON_WPARAM(wParam) == XBUTTON2) io.KeysDown[VK_XBUTTON2] = false;
            break;
        case WM_SYSKEYDOWN:
        case WM_KEYDOWN:
            if (wParam < 256)
                io.KeysDown[wParam] = true;
            break;
        case WM_SYSKEYUP:
        case WM_KEYUP:
            if (wParam < 256)
                io.KeysDown[wParam] = false;
            break;
        case WM_CHAR: // You can also use ToAscii()+GetKeyboardState() to retrieve characters.
            if (wParam > 0 && wParam < 0x10000)
                io.AddInputCharacter((unsigned short)wParam);
            break;
        default:
            break;
    }

    if (io.WantTextInput || io.WantCaptureMouse)
        return true;

    return CallWindowProc((WNDPROC)OldWndProc, hWnd, Message, wParam, lParam);
}

static LRESULT CALLBACK SafeWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    __try {
        return WndProc(hWnd, Message, wParam, lParam);
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        return CallWindowProc(reinterpret_cast<WNDPROC>(OldWndProc), hWnd, Message, wParam, lParam);
    }
}

enum MissionIcon
{
    MissionIcon_TyriaIncomplete,
    MissionIcon_TyriaPrimary,
    MissionIcon_TyriaBonus,
    MissionIcon_TyriaComplete,

    MissionIcon_CanthaIncomplete,
    MissionIcon_CanthaPrimary,
    MissionIcon_CanthaExpert,
    MissionIcon_CanthaMaster,

    MissionIcon_ElonaIncomplete,
    MissionIcon_ElonaPrimary,
    MissionIcon_ElonaExpert,
    MissionIcon_ElonaMaster,

    MissionIcon_DesolationIncomplete,
    MissionIcon_DesolationPrimary,
    MissionIcon_DesolationExpert,
    MissionIcon_DesolationMaster,

    MissionIcon_HmIncomplete,
    MissionIcon_HmPrimary,
    MissionIcon_HmBonus,
    MissionIcon_HmComplete,
    MissionIcon_HmMaster,

    MissionIcon_Count
};

static bool ArrayBoolAt(GW::Array<uint32_t>& array, uint32_t index)
{
    uint32_t real_index = index / 32;
    if (real_index >= array.size())
        return false;
    uint32_t shift = index % 32;
    uint32_t flag = 1 << shift;
    return (array[real_index] & flag) != 0;
}

static bool GetIsInTyria(GW::AreaInfo *area)
{
    switch (area->region)
    {
        case GW::Region_Kryta:
        case GW::Region_Maguuma:
        case GW::Region_Ascalon:
        case GW::Region_NorthernShiverpeaks:
        case GW::Region_CrystalDesert:
        case GW::Region_Presearing:
            return true;
        default:
            return false;
    }
}

static bool GetIsInCantha(GW::AreaInfo *area)
{
    switch (area->region)
    {
        case GW::Region_Kaineng:
        case GW::Region_Kurzick:
        case GW::Region_Luxon:
        case GW::Region_ShingJea:
            return true;
        default:
            return false;
    }
}

static bool GetIsInElona(GW::AreaInfo *area)
{
    switch (area->region)
    {
        case GW::Region_Kourna:
        case GW::Region_Vaabi:
        case GW::Region_Desolation:
        case GW::Region_Istan:
        case GW::Region_DomainOfAnguish:
            return true;
        default:
            return false;
    }
}

static bool GetIsInDesolation(GW::AreaInfo *area)
{
    switch (area->region)
    {
        case GW::Region_DomainOfAnguish:
            return true;
        default:
            return false;
    }
}

static IDirect3DTexture9 *mission_icon_textures[MissionIcon_Count];
static IDirect3DTexture9* GetMissionTexture(GW::WorldContext *ctx, GW::Constants::MapID map_id, bool hardmode)
{
    GW::Array<uint32_t>* missions_bonus;
    GW::Array<uint32_t>* missions_completed;

    MissionIcon incomplete;
    MissionIcon primary;
    MissionIcon expert;
    MissionIcon master;

    GW::AreaInfo *area_info = GW::Map::GetMapInfo(map_id);
    if (hardmode) {
        missions_bonus = &ctx->missions_bonus_hm;
        missions_completed = &ctx->missions_completed_hm;

        incomplete = MissionIcon_HmIncomplete;
        primary = MissionIcon_HmComplete;
        expert = MissionIcon_HmBonus;
        if (GetIsInTyria(area_info))
            master = MissionIcon_HmComplete;
        else
            master = MissionIcon_HmMaster;
    } else {
        missions_bonus = &ctx->missions_bonus;
        missions_completed = &ctx->missions_completed;

        if (GetIsInDesolation(area_info)) {
            incomplete = MissionIcon_DesolationIncomplete;
            primary = MissionIcon_DesolationPrimary;
            expert = MissionIcon_DesolationExpert;
            master = MissionIcon_DesolationMaster;
        } else if (GetIsInElona(area_info)) {
            incomplete = MissionIcon_ElonaIncomplete;
            primary = MissionIcon_ElonaPrimary;
            expert = MissionIcon_ElonaExpert;
            master = MissionIcon_ElonaMaster;
        } else if (GetIsInCantha(area_info)) {
            incomplete = MissionIcon_CanthaIncomplete;
            primary = MissionIcon_CanthaPrimary;
            expert = MissionIcon_CanthaExpert;
            master = MissionIcon_CanthaMaster;
        } else {
            incomplete = MissionIcon_TyriaIncomplete;
            primary = MissionIcon_TyriaPrimary;
            expert = MissionIcon_TyriaBonus;
            master = MissionIcon_TyriaComplete;
        }
    }

    bool bonus = ArrayBoolAt(*missions_bonus, static_cast<uint32_t>(map_id));
    bool complete = ArrayBoolAt(*missions_completed, static_cast<uint32_t>(map_id));

    if (bonus && complete)
        return mission_icon_textures[master];
    if (bonus)
        return mission_icon_textures[expert];
    if (complete)
        return mission_icon_textures[primary];

    return mission_icon_textures[incomplete];
}

static void WorldInformation_Draw(IDirect3DDevice9 *device)
{
    // This is call from within the game thread and all operation should be done here.
    // You can't freeze this thread, so no blocking operation or at your own risk.
    static bool initialized = false;
    if (!initialized) {

        HWND hWnd = GW::MemoryMgr::GetGWWindowHandle();
        OldWndProc = SetWindowLongPtr(hWnd, GWL_WNDPROC, reinterpret_cast<long>(SafeWndProc));

        ImGui::CreateContext();
        ImGui_ImplDX9_Init(hWnd, device);
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDrawCursor = false;

        map_names.resize(n_missions);
        for (uint32_t mission_id = 0; mission_id < n_missions; mission_id++) {
            GW::Constants::MapID map_id = missions[mission_id];
            GW::AreaInfo *area_info = GW::Map::GetMapInfo(map_id);
            if (!area_info) continue;
            if (area_info->type != GW::RegionType::MissionOutpost) continue;

            wchar_t enc_name[16];
            if (GW::UI::UInt32ToEncStr(area_info->name_id, enc_name, 16)) {
                GW::UI::AsyncDecodeStr(enc_name, &map_names[mission_id]);
            }
        }

        D3DXCreateTextureFromFileW(device, L"D:\\GWToolboxpp\\Dependencies\\GWCA\\Examples\\WorldInformation\\ressources\\MissionIconIncomplete.png", &mission_icon_textures[MissionIcon_TyriaIncomplete]);
        D3DXCreateTextureFromFileW(device, L"D:\\GWToolboxpp\\Dependencies\\GWCA\\Examples\\WorldInformation\\ressources\\MissionIconPrimary.png", &mission_icon_textures[MissionIcon_TyriaPrimary]);
        D3DXCreateTextureFromFileW(device, L"D:\\GWToolboxpp\\Dependencies\\GWCA\\Examples\\WorldInformation\\ressources\\94px-MissionIconBonus.png", &mission_icon_textures[MissionIcon_TyriaBonus]);
        D3DXCreateTextureFromFileW(device, L"D:\\GWToolboxpp\\Dependencies\\GWCA\\Examples\\WorldInformation\\ressources\\MissionIcon.png", &mission_icon_textures[MissionIcon_TyriaComplete]);

        D3DXCreateTextureFromFileW(device, L"D:\\GWToolboxpp\\Dependencies\\GWCA\\Examples\\WorldInformation\\ressources\\FactionsMissionIconIncomplete.png", &mission_icon_textures[MissionIcon_CanthaIncomplete]);
        D3DXCreateTextureFromFileW(device, L"D:\\GWToolboxpp\\Dependencies\\GWCA\\Examples\\WorldInformation\\ressources\\FactionsMissionIconPrimary.png", &mission_icon_textures[MissionIcon_CanthaPrimary]);
        D3DXCreateTextureFromFileW(device, L"D:\\GWToolboxpp\\Dependencies\\GWCA\\Examples\\WorldInformation\\ressources\\94px-FactionsMissionIconExpert.png", &mission_icon_textures[MissionIcon_CanthaExpert]);
        D3DXCreateTextureFromFileW(device, L"D:\\GWToolboxpp\\Dependencies\\GWCA\\Examples\\WorldInformation\\ressources\\FactionsMissionIcon.png", &mission_icon_textures[MissionIcon_CanthaMaster]);

        D3DXCreateTextureFromFileW(device, L"D:\\GWToolboxpp\\Dependencies\\GWCA\\Examples\\WorldInformation\\ressources\\NightfallMissionIconIncomplete.png", &mission_icon_textures[MissionIcon_ElonaIncomplete]);
        D3DXCreateTextureFromFileW(device, L"D:\\GWToolboxpp\\Dependencies\\GWCA\\Examples\\WorldInformation\\ressources\\94px-NightfallMissionIconPrimary.png", &mission_icon_textures[MissionIcon_ElonaPrimary]);
        D3DXCreateTextureFromFileW(device, L"D:\\GWToolboxpp\\Dependencies\\GWCA\\Examples\\WorldInformation\\ressources\\94px-NightfallMissionIconExpert.png", &mission_icon_textures[MissionIcon_ElonaExpert]);
        D3DXCreateTextureFromFileW(device, L"D:\\GWToolboxpp\\Dependencies\\GWCA\\Examples\\WorldInformation\\ressources\\120px-NightfallMissionIcon.png", &mission_icon_textures[MissionIcon_ElonaMaster]);

        D3DXCreateTextureFromFileW(device, L"D:\\GWToolboxpp\\Dependencies\\GWCA\\Examples\\WorldInformation\\ressources\\NightfallTormentMissionIconIncomplete.png", &mission_icon_textures[MissionIcon_DesolationIncomplete]);
        D3DXCreateTextureFromFileW(device, L"D:\\GWToolboxpp\\Dependencies\\GWCA\\Examples\\WorldInformation\\ressources\\94px-NightfallTormentMissionIconPrimary.png", &mission_icon_textures[MissionIcon_DesolationPrimary]);
        D3DXCreateTextureFromFileW(device, L"D:\\GWToolboxpp\\Dependencies\\GWCA\\Examples\\WorldInformation\\ressources\\94px-NightfallTormentMissionIconExpert.png", &mission_icon_textures[MissionIcon_DesolationExpert]);
        D3DXCreateTextureFromFileW(device, L"D:\\GWToolboxpp\\Dependencies\\GWCA\\Examples\\WorldInformation\\ressources\\120px-NightfallTormentMissionIcon.png", &mission_icon_textures[MissionIcon_DesolationMaster]);

        D3DXCreateTextureFromFileW(device, L"D:\\GWToolboxpp\\Dependencies\\GWCA\\Examples\\WorldInformation\\ressources\\HardModeMissionIconIncomplete.png", &mission_icon_textures[MissionIcon_HmIncomplete]);
        D3DXCreateTextureFromFileW(device, L"D:\\GWToolboxpp\\Dependencies\\GWCA\\Examples\\WorldInformation\\ressources\\94px-HardModeMissionIcon1.png", &mission_icon_textures[MissionIcon_HmPrimary]);
        D3DXCreateTextureFromFileW(device, L"D:\\GWToolboxpp\\Dependencies\\GWCA\\Examples\\WorldInformation\\ressources\\94px-HardModeMissionIcon1b.png", &mission_icon_textures[MissionIcon_HmBonus]);
        D3DXCreateTextureFromFileW(device, L"D:\\GWToolboxpp\\Dependencies\\GWCA\\Examples\\WorldInformation\\ressources\\94px-HardModeMissionIcon2.png", &mission_icon_textures[MissionIcon_HmComplete]);
        D3DXCreateTextureFromFileW(device, L"D:\\GWToolboxpp\\Dependencies\\GWCA\\Examples\\WorldInformation\\ressources\\94px-HardModeMissionIcon.png", &mission_icon_textures[MissionIcon_HmMaster]);

        GW::Chat::WriteChat(GW::Chat::CHANNEL_MODERATOR, L"WorldInformation: Initialized");
        initialized = true;
    }

    ImGui_ImplDX9_NewFrame();
    ImGui::GetIO().KeysDown[VK_CONTROL] = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
    ImGui::GetIO().KeysDown[VK_SHIFT] = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
    ImGui::GetIO().KeysDown[VK_MENU] = (GetKeyState(VK_MENU) & 0x8000) != 0;

    bool show_window = true;

    ImVec2 window_size(575.f, 600.f);
    ImGui::SetNextWindowSize(window_size);
    if (ImGui::Begin("World Information", &show_window, ImGuiWindowFlags_AlwaysAutoResize)) {
        /* Search bar header */
        static char search_buffer[256];
        ImGui::PushItemWidth(375.f);
        if (ImGui::InputText("", search_buffer, 256, ImGuiInputTextFlags_EnterReturnsTrue)) {
            GW::Chat::WriteChat(GW::Chat::CHANNEL_MODERATOR, L"Pressed enter");
        }
        ImGui::SameLine();
        if (ImGui::Button("Search", ImVec2(80.0f, 0))) {
            GW::Chat::WriteChat(GW::Chat::CHANNEL_MODERATOR, L"Press search");
        }
        ImGui::SameLine();
        if (ImGui::Button("Clear", ImVec2(80.0f, 0))) {
            strcpy_s(search_buffer, sizeof(search_buffer), "");
            GW::Chat::WriteChat(GW::Chat::CHANNEL_MODERATOR, L"Clear");
        }
        ImGui::Separator();

        static float unlocked_mission_grid_x[] = { 0, 75, 275, 375, 475 };
        ImGui::Text("Map Id");
        ImGui::SameLine(unlocked_mission_grid_x[1]);
        ImGui::Text("Map Name");
        ImGui::SameLine(unlocked_mission_grid_x[2]);
        ImGui::Text("Unlocked");
        ImGui::SameLine(unlocked_mission_grid_x[3]);
        ImGui::Text("Normal mode");
        ImGui::SameLine(unlocked_mission_grid_x[4]);
        ImGui::Text("Hard mode");
        ImGui::Spacing();

        static const float margin_height = 20.f;

        float win_x = ImGui::GetWindowPos().x;
        float win_y = ImGui::GetWindowPos().y;

        GW::WorldContext *ctx = GW::GameContext::instance()->world;
        for (uint32_t mission_id = 0; mission_id < n_missions; mission_id++) {
            GW::Constants::MapID map_id = missions[mission_id];

            ImGui::Dummy(ImVec2(0.0f, margin_height));
            ImGui::Text("%lu", static_cast<uint32_t>(map_id));
            ImGui::SameLine(unlocked_mission_grid_x[1]);
            ImGui::Text("%S", map_names[mission_id].c_str());
            ImGui::SameLine(unlocked_mission_grid_x[2]);
            if (ArrayBoolAt(ctx->unlocked_map, static_cast<uint32_t>(map_id))) {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
                ImGui::Text("yes");
            } else {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
                ImGui::Text("no");
            }
            ImGui::PopStyleColor();

            IDirect3DTexture9 *texture;
            {
                ImGui::SameLine(unlocked_mission_grid_x[3]);
                ImVec2 cursor = ImGui::GetCurrentWindow()->DC.CursorPos;
                float height = ImGui::GetTextLineHeight() + (margin_height / 1.f);
                texture = GetMissionTexture(ctx, map_id, false);
                ImVec2 tl(cursor.x - margin_height + 25.f, cursor.y - margin_height);
                ImVec2 br(tl.x + height, tl.y + height);
                ImGui::GetWindowDrawList()->AddImage(texture, tl, br);
            }
            {
                ImGui::SameLine(unlocked_mission_grid_x[4]);
                ImVec2 cursor = ImGui::GetCurrentWindow()->DC.CursorPos;
                float height = ImGui::GetTextLineHeight() + margin_height;
                texture = GetMissionTexture(ctx, map_id, true);
                ImVec2 tl(cursor.x - margin_height + 25.f, cursor.y - margin_height);
                ImVec2 br(tl.x + height, tl.y + height);
                ImGui::GetWindowDrawList()->AddImage(texture, tl, br);
            }

            ImGui::Spacing();
        }
        ImGui::End();
    }

    ImGui::Render();
    ImDrawData* draw_data = ImGui::GetDrawData();
    ImGui_ImplDX9_RenderDrawData(draw_data);

    if ((GetAsyncKeyState(VK_END) & 1) || !show_window) {
        HWND hWnd = GW::MemoryMgr::GetGWWindowHandle();

        ImGui_ImplDX9_Shutdown();
        ImGui::DestroyContext();
        SetWindowLongPtr(hWnd, GWL_WNDPROC, OldWndProc);

        GW::Chat::WriteChat(GW::Chat::CHANNEL_MODERATOR, L"WorldInformation: Bye!");
        GW::DisableHooks();

        for (uint32_t i = 0; i < MissionIcon_Count; i++) {
            mission_icon_textures[i]->Release();
            mission_icon_textures[i] = nullptr;
        }

        running = false;
    }
}

static DWORD WINAPI ThreadProc(LPVOID lpModule)
{
    // This is a new thread so you should only initialize GWCA and setup the hook on the game thread.
    // When the game thread hook is setup (i.e. SetRenderCallback), you should do the next operations
    // on the game from within the game thread.

    HMODULE hModule = static_cast<HMODULE>(lpModule);

    GW::Initialize();

    GW::Render::SetRenderCallback(WorldInformation_Draw);
    GW::Render::SetResetCallback([](IDirect3DDevice9* device) {
        ImGui_ImplDX9_InvalidateDeviceObjects();
    });

    running = true;
    while (running) {
        Sleep(100);
    }

    // Hooks are disable from Guild Wars thread (safely), so we just make sure we exit the last hooks
    while (GW::HookBase::GetInHookCount())
        Sleep(16);

    // We can't guarantee that the code in Guild Wars thread isn't still in the trampoline, but
    // practically a short sleep is fine.
    Sleep(16);
    GW::Terminate();

    FreeLibraryAndExitThread(hModule, EXIT_SUCCESS);
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    DisableThreadLibraryCalls(hModule);

    if (dwReason == DLL_PROCESS_ATTACH) {
        HANDLE handle = CreateThread(0, 0, ThreadProc, hModule, 0, 0);
        CloseHandle(handle);
    }

    return TRUE;
}
