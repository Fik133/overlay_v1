#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include "ui.hpp"
#include <vector>

bool windows_initialized = false;

std::vector<UIWindow> activeWindows;

void init_windows() {
	if (windows_initialized)
		return;
	activeWindows.emplace_back(
		"Media-Player",
		ui::windows::mediaplayer,
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize,
		UIWindowFlags_ShowTitleCollapsed,
		true, 
		ImVec2(300, 0),
		ImVec2(300, FLT_MAX)
	);
	activeWindows.emplace_back(
		"Demo",
		ui::windows::topbar,
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize,
		UIWindowFlags_ShowTitleCollapsed | UIWindowFlags_NoCollapse,
		true, 
		ImVec2(700, 0),
		ImVec2(700, FLT_MAX)
	);

	windows_initialized = true;
}

bool RenderUI() {

	init_windows();

	for (auto &active : activeWindows)
	{
		active.display_content();
	}

	return true;
}