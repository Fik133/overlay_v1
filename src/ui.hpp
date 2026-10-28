#pragma once
#include <imgui.h>
#include <functional>
#include <string>
using FContent = std::function<void()>;


namespace styles {
	namespace ui {
		void dark_color();
	}
}

inline ImVec4 ChangeAlpha(ImVec4 vector, float alpha) {
	return ImVec4(vector.x, vector.y, vector.z, alpha);
}

struct UIWindow {
	UIWindow(const char* _title, FContent _content, ImGuiWindowFlags _flags, bool _showed = true, ImVec2 _min_size = ImVec2(0,0), ImVec2 _max_size = ImVec2(FLT_MAX,FLT_MAX)) :
		title(_title), showed(_showed), content(_content), flags(_flags),  min_size(_min_size), max_size(_max_size) {}
	bool showed = true;
	const char* title = "";
	FContent content = nullptr; 
	ImGuiWindowFlags flags;
	ImVec2 min_size = ImVec2(0,0);
	ImVec2 max_size = ImVec2(FLT_MAX,FLT_MAX);

	bool display_content() noexcept;
	bool set_visible(bool _showed) noexcept {
		showed = _showed;
	}
};

template<typename T>
struct UIProgressBar {
	T v_max, v_min, v_current;
	float width = 100.f;
	float height = 4.f;
	
	void set_max(T _max) noexcept { v_max = _max; }
	void set_min(T _min) noexcept { v_min = _min; }
	void set_current(T _current) noexcept { v_current = _current; }
	void set_width(float _width) noexcept { width = _width; }
	void set_height(float _height) noexcept { height = _height; }

	void render();

};

namespace ui {
	namespace windows {
		void mediaplayer();
	}
}

template<typename T>
inline void UIProgressBar<T>::render()
{
	ImVec2 cursor = ImGui::GetCursorScreenPos();
	ImGui::PushID(this);
	ImGui::InvisibleButton("progress_bar", ImVec2(width, height));
	ImGui::PopID();
	auto drawlist = ImGui::GetWindowDrawList();

	auto style = ImGui::GetStyle();
	float rounding = style.FrameRounding;

	drawlist->AddRectFilled(cursor, ImVec2(cursor.x + width, cursor.y + height), ImColor(ChangeAlpha(ImGui::GetStyle().Colors[ImGuiCol_SliderGrabActive], 0.3f)), rounding);

	drawlist->AddRectFilled(cursor, ImVec2(cursor.x + (v_current/(v_max - v_min))*width, cursor.y + height), ImColor(ChangeAlpha(ImGui::GetStyle().Colors[ImGuiCol_SliderGrabActive], 1.f)), rounding);
}
