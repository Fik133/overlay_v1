#include "ui.hpp"
#include "globals.hpp"
#include <imgui.h>
#include <math.h>

bool selectable(const char* label, bool* p_selected = false) {

    ImGui::Text(label);

    bool result = ImGui::IsItemClicked();

    bool& selected = *p_selected;

    if (result) {
        selected = !selected;
    }

    if (ImGui::IsItemHovered()) {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    }

    return result;
}

bool UIWindow::display_content() noexcept {
	if (content == nullptr)
		return false;

    ImVec2 max_window_size = { FLT_MAX, FLT_MAX };
    ImVec2 min_win_size = { 0 , 0 };

    if (max_window_size.x != this->max_size.x || max_window_size.y != this->max_size.y) {

        this->max_size.x = std::abs(this->max_size.x);
        this->max_size.y = std::abs(this->max_size.y);

        max_window_size = this->max_size;
    }

    if (min_win_size.x != this->min_size.x || min_win_size.y != this->min_size.y) {
        this->min_size.x = std::abs(this->min_size.x);
        this->min_size.y = std::abs(this->min_size.y);

        min_win_size = this->min_size;
    }


    if (!this->visible_passthrough && g_isPassthrough)
        return true; 

    if (this->collapsed) {
        this->opacity_factor = OPACITY_COLLAPSED;
    }
    else {
        this->opacity_factor = 1.f;
    }

    ImVec4 window_bg = ImGui::GetStyleColorVec4(ImGuiCol_WindowBg);

    ImVec4 new_style = ChangeAlpha(window_bg, window_bg.w * opacity_factor);

    ImGui::PushStyleColor(ImGuiCol_WindowBg, new_style);
    ImGui::SetNextWindowSizeConstraints(min_win_size, max_window_size);
	ImGui::Begin(this->title, &this->showed, this->flags);

    bool show_collapse_menu = !(this->window_flags & UIWindowFlags_NoCollapse);

    if ((this->window_flags & UIWindowFlags_ShowTitleCollapsed) && (this->collapsed || ImGui::IsWindowHovered() && !g_isPassthrough) && show_collapse_menu) {
        ImGui::Text(this->title);
        ImGui::SameLine();
    }

    if (!g_isPassthrough && show_collapse_menu) {

        if (ImGui::IsWindowHovered()) {

            float spacing_x = ImGui::GetStyle().ItemSpacing.x;

            float available_x = ImGui::GetContentRegionAvail().x;

            const char* text = this->visible_passthrough ? ICON_FA_EYE : ICON_FA_EYE_SLASH;

            float text_size_x = ImGui::CalcTextSize(text).x;

            float next_item_position = ImGui::GetCursorPosX() + available_x - text_size_x;

            ImGui::SetCursorPosX(next_item_position);

            selectable(text, &this->visible_passthrough);

            text = this->collapsed ? ICON_FA_CHEVRON_UP : ICON_FA_CHEVRON_DOWN;

            ImGui::SameLine();

            text_size_x = ImGui::CalcTextSize(text).x;

            ImGui::SetCursorPosX(next_item_position - text_size_x - ImGui::GetStyle().ItemSpacing.x);

            selectable(text, &this->collapsed);

            if(!this->collapsed)
                ImGui::Separator();
        }
    }

    if ((!this->collapsed)) {
        this->content();
    }

	ImGui::End();

    ImGui::PopStyleColor(1);

	return true;
}

void styles::ui::dark_color() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    colors[ImGuiCol_Text]                   = ImVec4(0.95f, 0.95f, 0.97f, 1.00f); 
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.55f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.11f, 0.11f, 0.13f, 0.80f); 
    colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.13f, 0.13f, 0.16f, 0.94f);
    colors[ImGuiCol_Border]                 = ImVec4(0.25f, 0.25f, 0.30f, 0.50f); 
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

    colors[ImGuiCol_FrameBg]                = ImVec4(0.18f, 0.18f, 0.22f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.25f, 0.25f, 0.31f, 1.00f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.30f, 0.30f, 0.38f, 1.00f);

    colors[ImGuiCol_SliderGrab]             = ImVec4(0.10f, 0.80f, 0.40f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.20f, 0.90f, 0.50f, 1.00f);

    colors[ImGuiCol_Button]                 = ImVec4(0.20f, 0.20f, 0.25f, 1.00f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.28f, 0.28f, 0.35f, 1.00f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.10f, 0.80f, 0.40f, 1.00f);

    colors[ImGuiCol_Header]                 = ImVec4(0.20f, 0.20f, 0.25f, 1.00f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.28f, 0.28f, 0.35f, 1.00f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.10f, 0.80f, 0.40f, 1.00f);

    style.WindowRounding    = 10.0f; 
    style.ChildRounding     = 6.0f;
    style.FrameRounding     = 6.0f; 
    style.PopupRounding     = 6.0f;
    style.ScrollbarRounding = 9.0f;
    style.GrabRounding      = 6.0f;
 
    style.WindowPadding     = ImVec2(12.0f, 12.0f);
    style.FramePadding      = ImVec2(8.0f, 6.0f);
    style.ItemSpacing       = ImVec2(8.0f, 8.0f);
}