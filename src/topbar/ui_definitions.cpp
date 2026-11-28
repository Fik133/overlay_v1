#include "topbar.h"
#include <numbers>
#include "../globals.hpp"

bool ui::topbar::TabButton(const char * text, const char * icon, bool sameline, ImVec2 size, std::function<void()> event)
{
	std::string button_text = std::string(icon) + " " + std::string(text);
	bool clicked = ImGui::Button(button_text.c_str(), size);

	if (ImGui::IsItemHovered()) {
		ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
	}

	if (sameline) {
		ImGui::SameLine();
	}

	if (clicked && event != nullptr) {
		event();
	}

	return clicked;
}

bool ui::topbar::ModalOpenButton(const char* text, const char* icon, const char* modal, FContent content)
{
	const bool result = TabButton(text, icon);
	if (result) {
		ImGui::OpenPopup(modal);
	}
	modal::HandleModal(modal, content);
	return result;
}

bool ui::topbar::modal::SettingButton(const char * text, const char * icon, float width)
{
	bool result = ui::topbar::TabButton(text, icon, false, ImVec2(width, 0));

	return result;
}

void ui::topbar::modal::HandleModal(const char* modal, FContent content)
{
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();

	float window_size = g_FrameBufferSize.x/3;

	ImGui::SetNextWindowSize(ImVec2(window_size, window_size/std::numbers::phi)); //golden ratio

	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	ImGuiWindowFlags popup_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar;

	if (ImGui::BeginPopupModal(modal, nullptr, popup_flags)) {

		content();
		ImGui::EndPopup();
	}
}

