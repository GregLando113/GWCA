#pragma once

#include <imgui.h>

namespace ImGui {
    // Shows '(?)' and the helptext when hovered
    IMGUI_API void ShowHelp(const char* help);

    IMGUI_API bool MyCombo(const char* label, const char* preview_text, int* current_item, 
        bool(*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int height_in_items = -1);

    IMGUI_API bool ColorPalette(const char *label, size_t *palette_index, ImVec4 *palette, size_t count, size_t max_per_line = (size_t)-1, ImGuiColorEditFlags flags = 0);
}
