#include "utils.h"
#include <imgui.h>
#include <format>

void ui::tools::AlignRight(float width)
{
	float target_x = ImGui::GetContentRegionMax().x - width;
	ImGui::SetCursorPosX(target_x);
}

void ui::tools::TextAlignRight(const char* text, float max_width, float offset)
{
	float oldX = ImGui::GetCursorPosX();

	AlignRight(max_width + (offset) + (offset == 0 ? 0 : ImGui::GetStyle().ItemSpacing.x));
	ImGui::TextUnformatted(text);

	ImGui::SetCursorPosX(oldX);

	ImGui::SameLine();
	ImGui::Dummy(ImVec2(0, 0));
}

