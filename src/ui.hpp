#pragma once
#include <imgui.h>
#include <functional>

using FContent = std::function<void()>;

struct UIWindow {
	UIWindow(const char* _title, FContent _content, ImGuiWindowFlags _flags, bool _showed = true) :
		title(_title), showed(_showed), content(_content), flags(_flags)  {}
	bool showed = true;
	const char* title = "";
	FContent content = nullptr; 
	ImGuiWindowFlags flags;

	bool display_content() noexcept;
	bool set_visible(bool _showed) noexcept {
		showed = _showed;
	}
};

namespace ui {
	namespace windows {
		void mediaplayer();
	}
}