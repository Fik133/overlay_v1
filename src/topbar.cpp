
#include "ui.hpp"
#include <imgui.h>
#include "../external/include/IconsFontAwesome7.h"
#include <string>

bool tb_button(const char* text, const char* icon) {
	std::string button_text = std::string(icon) + " " + std::string(text);
	bool clicked = ImGui::Button(button_text.c_str());

	if (ImGui::IsItemHovered()) {
		ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
	}

	ImGui::SameLine();

	return clicked;
}

void align_right(float width)
{
	float target_x = ImGui::GetContentRegionMax().x - width;
	ImGui::SetCursorPosX(target_x);
}

void tb_text_value_right(const char* text, const char* icon, float value)
{
	std::string text_value =
		std::string(icon) + " " +
		std::string(text) + ": " +
		std::to_string(static_cast<int>(std::round(value)));

	float text_size = ImGui::CalcTextSize(text_value.c_str()).x;

	align_right(text_size);
	ImGui::TextUnformatted(text_value.c_str());


}

void ui::windows::topbar()
{

	ImGui::BeginChild("TOPBAR", ImVec2(0,0), ImGuiChildFlags_AutoResizeY);
	
	tb_button("Whiteboard", ICON_FA_CHALKBOARD);
	tb_button("Widgets", ICON_FA_GRIP);
	tb_button("Settings", ICON_FA_GEARS);


	float value = 1.f / ImGui::GetIO().DeltaTime;
	tb_text_value_right("FPS", ICON_FA_GAUGE, value);

	ImGui::EndChild();
}
