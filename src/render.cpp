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
	activeWindows.emplace_back("spotify#1", ui::windows::mediaplayer, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize, true, 
		ImVec2(300, 0), ImVec2(300, FLT_MAX));

	windows_initialized = true;
}

bool RenderUI() {

	init_windows();

	for (auto active : activeWindows)
	{
		active.display_content();
	}

	return true;
}