#include "ui.hpp"
#include <imgui.h>
bool UIWindow::display_content() noexcept {
	if (content == nullptr)
		return false;

	ImGui::Begin(this->title, &this->showed, this->flags);
	this->content();
	ImGui::End();

	return true;
}

