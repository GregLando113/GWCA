#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Windowsx.h>

#include <cassert>
#include <cstdint>

#include <string>
#include <vector>

#include <imgui.h>
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "ImGuiAddons.h"
#include "imgui_impl_dx9.h"
#include <imgui_internal.h>

#include <GWCA/GWCA.h>

#include <GWCA/Constants/Constants.h>
#include <GWCA/Utilities/Hooker.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/GameEntities/Agent.h>

#include <GWCA/Managers/AgentMgr.h>
#include <GWCA/Managers/ChatMgr.h>
#include <GWCA/Managers/MemoryMgr.h>
#include <GWCA/Managers/RenderMgr.h>

#include "ArmorsDatabase.h"


// Arg3:
//  - Costume = 0x20000006
//  - Armor =   0x20110007 (3)
//  - Chaos glove = 0x20110407 (38)
typedef void (__fastcall *SetItem_pt)(GW::Equipment *equip, void *edx, uint32_t model_file_id, uint32_t color, uint32_t arg3, uint32_t agent_id);

enum class DyeColor {
    None = 0,
    Blue = 2,
    Green = 3,
    Purple = 4,
    Red = 5,
    Yellow = 6,
    Brown = 7,
    Orange = 8,
    Silver = 9,
    Black = 10,
    Gray = 11,
    White = 12,
    Pink = 13
};
static ImVec4 palette[] = {
    {1.f, 0.f, 0.f, 0.f},       // Blue
    {0.f, 0.75f, 0.f, 0.f},     // Green
    {0.5f, 0.f, 0.5f, 0.f},     // Purple
    {0.f, 0.f, 1.f, 0.f},       // Red
    {0.f, 1.f, 1.f, 0.f},       // Yellow
    {0.f, 0.25f, 0.5f, 0.f},    // Brown
    {0.f, 0.65f, 1.f, 0.f},     // Orange
    {0.75f, 0.75f, 0.75f, 0.f}, // Silver
    {0.f, 0.f, 0.f, 0.f},       // Black
    {0.5f, 0.5f, 0.5f, 0.f},    // Gray
    {1.f, 1.f, 1.f, 0.f},       // White
    {0.95f, 0.5f, 0.95f, 0.f},  // Pink
};

static DyeColor DyeColorFromInt(size_t color)
{
    DyeColor col = static_cast<DyeColor>(color);
    switch (col) {
    case DyeColor::None:
    case DyeColor::Blue:
    case DyeColor::Green:
    case DyeColor::Purple:
    case DyeColor::Red:
    case DyeColor::Yellow:
    case DyeColor::Brown:
    case DyeColor::Orange:
    case DyeColor::Silver:
    case DyeColor::Black:
    case DyeColor::Gray:
    case DyeColor::White:
    case DyeColor::Pink:
        return col;
    default:
        return DyeColor::None;
    }
}

static ImVec4 ImVec4FromDyeColor(DyeColor color)
{
    uint32_t color_id = static_cast<uint32_t>(color) - static_cast<uint32_t>(DyeColor::Blue);
    switch (color) {
    case DyeColor::Blue:
    case DyeColor::Green:
    case DyeColor::Purple:
    case DyeColor::Red:
    case DyeColor::Yellow:
    case DyeColor::Brown:
    case DyeColor::Orange:
    case DyeColor::Silver:
    case DyeColor::Black:
    case DyeColor::Gray:
    case DyeColor::White:
    case DyeColor::Pink:
        assert(color_id < _countof(palette));
        return palette[color_id];
    default: return ImVec4();
    }
}

struct PlayerArmorPiece
{
    uint32_t model_file_id;
    uint32_t unknow1;
    DyeColor color1;
    DyeColor color2;
    DyeColor color3;
    DyeColor color4;
};

struct PlayerArmor
{
    PlayerArmorPiece head;
    PlayerArmorPiece chest;
    PlayerArmorPiece hands;
    PlayerArmorPiece legs;
    PlayerArmorPiece feets;
};

struct ComboListState
{
    std::vector<Armor *> pieces;
    int current_piece_index = -1;
    Armor *current_piece = nullptr;
};

static bool running;
static bool shutdown;
static long OldWndProc;
static SetItem_pt SetItem_Func;

static PlayerArmor player_armor;
static GW::Constants::Profession current_profession = GW::Constants::Profession::None;

static ComboListState head;
static ComboListState chest;
static ComboListState hands;
static ComboListState legs;
static ComboListState feets;

static const char* GetProfessionName(GW::Constants::Profession prof)
{
    switch (prof) {
    case GW::Constants::Profession::None: return "None";
    case GW::Constants::Profession::Warrior: return "Warrior";
    case GW::Constants::Profession::Ranger: return "Ranger";
    case GW::Constants::Profession::Monk: return "Monk";
    case GW::Constants::Profession::Necromancer: return "Necromancer";
    case GW::Constants::Profession::Mesmer: return "Mesmer";
    case GW::Constants::Profession::Elementalist: return "Elementalist";
    case GW::Constants::Profession::Assassin: return "Assassin";
    case GW::Constants::Profession::Ritualist: return "Ritualist";
    case GW::Constants::Profession::Paragon: return "Paragon";
    case GW::Constants::Profession::Dervish: return "Dervish";
    default: return "Unknow Profession";
    }
}

static GW::Constants::Profession GetAgentProfession(GW::AgentLiving *agent)
{
    if (!agent)
        return GW::Constants::Profession::None;
    GW::Constants::Profession primary = static_cast<GW::Constants::Profession>(agent->primary);
    switch (primary) {
    case GW::Constants::Profession::None:
    case GW::Constants::Profession::Warrior:
    case GW::Constants::Profession::Ranger:
    case GW::Constants::Profession::Monk:
    case GW::Constants::Profession::Necromancer:
    case GW::Constants::Profession::Mesmer:
    case GW::Constants::Profession::Elementalist:
    case GW::Constants::Profession::Assassin:
    case GW::Constants::Profession::Ritualist:
    case GW::Constants::Profession::Paragon:
    case GW::Constants::Profession::Dervish:
        return primary;
    default:
        return GW::Constants::Profession::None;
    }
}

static LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    static bool right_mouse_down = false;

    if (Message == WM_RBUTTONDOWN) right_mouse_down = true;
    if (Message == WM_RBUTTONDBLCLK) right_mouse_down = true;
    if (Message == WM_RBUTTONUP) right_mouse_down = false;

    ImGuiIO& io = ImGui::GetIO();

    //
    // The first switch case is used to update the state of imgui with respect of the inputs.
    //

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

    //
    // This second switch is used to determine whether we need to forward the input to Guild Wars.
    //

    switch (Message) {
    // Send button up mouse events to everything, to avoid being stuck on mouse-down
    case WM_LBUTTONUP:
        break;

    // Other mouse events:
    // - If right mouse down, leave it to gw
    // - ImGui first (above), if WantCaptureMouse that's it
    // - Toolbox module second (e.g.: minimap), if captured, that's it
    // - otherwise pass to gw
    case WM_LBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
    case WM_MOUSEMOVE:
    case WM_MOUSEWHEEL:
        if (!right_mouse_down && io.WantCaptureMouse)
            return true;
        break;

    // keyboard messages
    case WM_KEYUP:
    case WM_SYSKEYUP:
        if (io.WantTextInput) break; // if imgui wants them, send to imgui (above) and to gw
        [[fallthrough]];
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    case WM_CHAR:
    case WM_SYSCHAR:
    case WM_IME_CHAR:
    case WM_XBUTTONDOWN:
    case WM_XBUTTONDBLCLK:
    case WM_XBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONDBLCLK:
    case WM_MBUTTONUP:
        if (io.WantTextInput) return true; // if imgui wants them, send just to imgui (above)

        // note: capturing those events would prevent typing if you have a hotkey assigned to normal letters.
        // We may want to not send events to toolbox if the player is typing in-game
        // Otherwise, we may want to capture events.
        // For that, we may want to only capture *successfull* hotkey activations.
        break;

    case WM_SIZE:
        // ImGui doesn't need this, it reads the viewport size directly
        break;

    default:
        break;
    }

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

static bool armor_filter_array_getter(void* data, int idx, const char** out_text)
{
    switch (idx) {
    case Campaign_All: *out_text = "All"; break;
    case Campaign_Core: *out_text = "Core"; break;
    case Campaign_Prophecies: *out_text = "Prophecies"; break;
    case Campaign_Factions: *out_text = "Factions"; break;
    case Campaign_Nightfall: *out_text = "Nightfall"; break;
    case Campaign_EotN: *out_text = "Eye of the North"; break;
    default: return false;
    };
    return true;
}

static bool armor_pieces_array_getter(void* data, int idx, const char** out_text)
{
    Armor **armors = static_cast<Armor**>(data);
    *out_text = armors[idx]->label;
    return true;
}

static void UpdateArmorsFilter(GW::Constants::Profession prof, Campaign campaign)
{
    size_t count = 0;
    Armor *armors = GetArmorsPerProfession(prof, &count);

    head.pieces.clear();
    chest.pieces.clear();
    hands.pieces.clear();
    legs.pieces.clear();
    feets.pieces.clear();

    head.current_piece_index = -1;
    chest.current_piece_index = -1;
    hands.current_piece_index = -1;
    legs.current_piece_index = -1;
    feets.current_piece_index = -1;

    for (size_t i = 0; i < count; i++) {
        ComboListState *state = nullptr;
        PlayerArmorPiece *piece = nullptr;

        switch (armors[i].item_slot) {
        case ItemSlot_Head:
            state = &head;
            piece = &player_armor.head;
            break;
        case ItemSlot_Chest:
            state = &chest;
            piece = &player_armor.chest;
            break;
        case ItemSlot_Hands:
            state = &hands;
            piece = &player_armor.hands;
            break;
        case ItemSlot_Legs:
            state = &legs;
            piece = &player_armor.legs;
            break;
        case ItemSlot_Feets:
            state = &feets;
            piece = &player_armor.feets;
            break;
        }

        if (!(state && piece))
            continue;
        if (piece->model_file_id == armors[i].model_file_id)
            state->current_piece = &armors[i];
        if (campaign != Campaign_All && armors[i].campaign != campaign)
            continue;
        state->pieces.push_back(&armors[i]);
    }

    current_profession = prof;
}

static void InitItemPiece(PlayerArmorPiece *piece, GW::Equipment::ItemData *item_data)
{
    piece->model_file_id = item_data->model_file_id;
    piece->unknow1 = item_data->dye.dye_id;
    piece->color1 = DyeColorFromInt(item_data->dye.dye1);
    piece->color2 = DyeColorFromInt(item_data->dye.dye2);
    piece->color3 = DyeColorFromInt(item_data->dye.dye3);
    piece->color4 = DyeColorFromInt(item_data->dye.dye4);
}

static uint32_t CreateColor(DyeColor col1, DyeColor col2 = DyeColor::None,
    DyeColor col3 = DyeColor::None, DyeColor col4 = DyeColor::None)
{
    if (col1 == DyeColor::None && col2 == DyeColor::None && col3 == DyeColor::None && col4 == DyeColor::None)
        col1 = DyeColor::Gray;
    uint32_t c1 = static_cast<uint32_t>(col1);
    uint32_t c2 = static_cast<uint32_t>(col2);
    uint32_t c3 = static_cast<uint32_t>(col3);
    uint32_t c4 = static_cast<uint32_t>(col4);
    uint32_t composite = c1 | (c2 << 4) | (c3 << 8) | (c4 << 12);
    return composite;
}

static void SetArmorItem(PlayerArmorPiece *piece)
{
    assert(SetItem_Func != nullptr);
    GW::AgentLiving *player = GW::Agents::GetPlayerAsAgentLiving();
    if (!(player && player->equip && *player->equip))
        return;
    GW::Equipment *equip = *player->equip;
    uint32_t color = CreateColor(piece->color1, piece->color2, piece->color3, piece->color4);
    // 0x60111109
    SetItem_Func(*player->equip, nullptr, piece->model_file_id, color, 0x20110007, piece->unknow1);
}

static bool DyePicker(const char *label, DyeColor *color)
{
    ImGui::PushID(label);

    ImVec4 current_color = ImVec4FromDyeColor(*color);

    bool value_changed = false;
    const char* label_display_end = ImGui::FindRenderedTextEnd(label);

    if (ImGui::ColorButton("##ColorButton", current_color, *color == DyeColor::None ? ImGuiColorEditFlags_AlphaPreview : 0))
        ImGui::OpenPopup("picker");

    if (ImGui::BeginPopup("picker"))
    {
        if (label != label_display_end)
        {
            ImGui::TextUnformatted(label, label_display_end);
            ImGui::Separator();
        }
        size_t palette_index;
        if (ImGui::ColorPalette("##picker", &palette_index, palette, _countof(palette), 7, ImGuiColorEditFlags_AlphaPreview))
        {
            if (palette_index < _countof(palette))
                *color = DyeColorFromInt(palette_index + static_cast<size_t>(DyeColor::Blue));
            else
                *color = DyeColor::None;
            value_changed = true;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    ImGui::PopID();
    return value_changed;
}

static bool DrawArmorPiece(const char* label,
    PlayerArmorPiece *player_piece, ComboListState* state)
{
    ImGui::PushID(label);

    bool value_changed = false;
    const char *preview = state->current_piece ? state->current_piece->label : "";
    if (ImGui::MyCombo("##armors", preview, &state->current_piece_index,
        armor_pieces_array_getter, state->pieces.data(), state->pieces.size()))
    {
        if (0 <= state->current_piece_index && (size_t)state->current_piece_index < state->pieces.size()) {
            state->current_piece = state->pieces[state->current_piece_index];
            player_piece->model_file_id = state->current_piece->model_file_id;
            player_piece->unknow1 = state->current_piece->unknow1;
        }
        value_changed = true;
    }

    ImGui::SameLine();
    value_changed |= DyePicker("color1", &player_piece->color1);
    ImGui::SameLine();
    value_changed |= DyePicker("color2", &player_piece->color2);
    ImGui::SameLine();
    value_changed |= DyePicker("color3", &player_piece->color3);
    ImGui::SameLine();
    value_changed |= DyePicker("color4", &player_piece->color4);

    ImGui::PopID();
    return value_changed;
}

static void DrawGwArmory(IDirect3DDevice9* device)
{
    GW::AgentLiving* player_agent = GW::Agents::GetPlayerAsAgentLiving();
    if (!player_agent)
        return;

    bool update_data = false;
    GW::Constants::Profession prof = GetAgentProfession(player_agent);
    if (prof != current_profession)
        update_data = true;

    bool show_window = true;
    static int filter_index = Campaign_All;

    ImVec2 window_size(330.f, 208.f);
    ImGui::SetNextWindowSize(window_size);
    if (ImGui::Begin("GwArmory", &show_window, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Profession: %s", GetProfessionName(prof));
        ImGui::SameLine(ImGui::GetWindowWidth() - 65.f);

        if (ImGui::Button("Refresh")) {
            if (player_agent->equip && player_agent->equip[0]) {
                GW::Equipment *equip = player_agent->equip[0];
                InitItemPiece(&player_armor.head, &equip->head);
                InitItemPiece(&player_armor.chest, &equip->chest);
                InitItemPiece(&player_armor.hands, &equip->hands);
                InitItemPiece(&player_armor.legs, &equip->legs);
                InitItemPiece(&player_armor.feets, &equip->feet);
            }

            update_data = true;
        }

        if (ImGui::MyCombo("##filter", "All", &filter_index, armor_filter_array_getter, nullptr, 5))
            update_data = true;
        if (update_data)
            UpdateArmorsFilter(prof, static_cast<Campaign>(filter_index));

        if (DrawArmorPiece("##head", &player_armor.head, &head))
            SetArmorItem(&player_armor.head);
        if (DrawArmorPiece("##chest", &player_armor.chest, &chest))
            SetArmorItem(&player_armor.chest);
        if (DrawArmorPiece("##hands", &player_armor.hands, &hands))
            SetArmorItem(&player_armor.hands);
        if (DrawArmorPiece("##legs", &player_armor.legs, &legs))
            SetArmorItem(&player_armor.legs);
        if (DrawArmorPiece("##feets", &player_armor.feets, &feets))
            SetArmorItem(&player_armor.feets);
    }
    ImGui::End();
    shutdown = !show_window;
}

static void Draw(IDirect3DDevice9* device)
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

        GW::Scanner::Initialize();
        SetItem_Func = (SetItem_pt)GW::Scanner::Find("\x83\xC4\x04\x8B\x08\x8B\xC1\xC1", "xxxxxxxx", -0x24);
        if (!SetItem_Func) {
            GW::Chat::WriteChat(GW::Chat::CHANNEL_MODERATOR, L"GwArmory: Failed to find the SetItem function");
        } else {
            GW::AgentLiving* player_agent = GW::Agents::GetPlayerAsAgentLiving();
            if (player_agent && player_agent->equip && player_agent->equip[0]) {
                GW::Equipment *equip = player_agent->equip[0];
                InitItemPiece(&player_armor.head, &equip->head);
                InitItemPiece(&player_armor.chest, &equip->chest);
                InitItemPiece(&player_armor.hands, &equip->hands);
                InitItemPiece(&player_armor.legs, &equip->legs);
                InitItemPiece(&player_armor.feets, &equip->feet);
            }
        }

        GW::Chat::WriteChat(GW::Chat::CHANNEL_MODERATOR, L"GwArmory: Initialized");
        initialized = true;
    }

    ImGui_ImplDX9_NewFrame();
    ImGui::GetIO().KeysDown[VK_CONTROL] = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
    ImGui::GetIO().KeysDown[VK_SHIFT] = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
    ImGui::GetIO().KeysDown[VK_MENU] = (GetKeyState(VK_MENU) & 0x8000) != 0;

    DrawGwArmory(device);

    ImGui::Render();
    ImDrawData* draw_data = ImGui::GetDrawData();
    ImGui_ImplDX9_RenderDrawData(draw_data);

#ifdef _DEBUG
    if (GetAsyncKeyState(VK_END) & 1)
        shutdown = true;
#endif

    if (shutdown) {
        HWND hWnd = GW::MemoryMgr::GetGWWindowHandle();

        ImGui_ImplDX9_Shutdown();
        ImGui::DestroyContext();
        SetWindowLongPtr(hWnd, GWL_WNDPROC, OldWndProc);

        GW::Chat::WriteChat(GW::Chat::CHANNEL_MODERATOR, L"GwArmory: Bye!");
        GW::DisableHooks();
        running = false;
    }
}

static DWORD WINAPI ThreadProc(LPVOID lpModule)
{
    // This is a new thread so you should only initialize GWCA and setup the hook on the game thread.
    // When the game thread hook is setup (i.e. SetRenderCallback), you should do the next operations
    // on the game from within the game thread.

    HMODULE hModule = static_cast<HMODULE>(lpModule);

    FILE* stdout_proxy = nullptr;
    FILE* stderr_proxy = nullptr;
#ifdef _DEBUG
    AllocConsole();
    SetConsoleTitle("GwArmory Console");
    freopen_s(&stdout_proxy, "CONOUT$", "w", stdout);
    freopen_s(&stderr_proxy, "CONOUT$", "w", stderr);
#else
#if 0
    // If you replace the above "#if 0" by "#if 1", you will log
    // the stdout in "log.txt" which will be in your "Gw.exe" folder.
    freopen_s(&stdout_proxy, "log.txt", "w", stdout);
#endif
#endif

    GW::Initialize();

    GW::Render::SetRenderCallback(Draw);
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

    if (stdout_proxy)
        fclose(stdout_proxy);
    if (stderr_proxy)
        fclose(stderr_proxy);
    FreeConsole();

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
