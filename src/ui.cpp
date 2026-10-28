#include "ui.hpp"
#include <imgui.h>
#include <math.h>
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

    ImGui::SetNextWindowSizeConstraints(min_win_size, max_window_size);
	ImGui::Begin(this->title, &this->showed, this->flags);
	this->content();
	ImGui::End();

	return true;
}

void styles::ui::dark_color() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    colors[ImGuiCol_Text]                   = ImVec4(0.95f, 0.95f, 0.97f, 1.00f); 
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.55f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.11f, 0.11f, 0.13f, 0.94f); 
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