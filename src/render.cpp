#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include "ui.hpp"
#include "globals.hpp"
#include <vector>


#ifdef _WIN32
#include <Windows.h>
#endif

bool windows_initialized = false;

std::vector<UIWindow> activeWindows;

void init_windows() {
	if (windows_initialized)
		return;

	float topbar_x_position = 0;
	float topbar_width = 0;
	{
		topbar_width = g_FrameBufferSize.x / 2;
		topbar_x_position = (g_FrameBufferSize.x / 2.f) - (topbar_width / 2.f);
	}

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
		"TopBar Widget",
		ui::windows::topbar,
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove,
		UIWindowFlags_NoCollapse | UIWindowFlags_SetInitialPosition  | UIWindowFlags_HideOnPassthrough,
		true, 
		ImVec2(topbar_width, 0),
		ImVec2(topbar_width, FLT_MAX),
		ImVec2(topbar_x_position, 0)
	);

	activeWindows.emplace_back(
		"Settings",
		ui::windows::demo,
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize,
		UIWindowFlags_ShowTitleCollapsed,
		false, 
		ImVec2(400, 0),
		ImVec2(400, FLT_MAX)
	);

	activeWindows.emplace_back(
		"Whiteboard Widget",
		ui::windows::demo,
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize,
		UIWindowFlags_ShowTitleCollapsed,
		false, 
		ImVec2(400, 0),
		ImVec2(400, FLT_MAX)
	);

	activeWindows.emplace_back(
		"Widget Settings",
		ui::windows::demo,
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize,
		UIWindowFlags_ShowTitleCollapsed,
		false, 
		ImVec2(400, 0),
		ImVec2(400, FLT_MAX)
	);

	windows_initialized = true;
}

bool RenderUI() {

	init_windows();

	for (auto &active : activeWindows)
	{
		active.display_content();
	}

	if (g_showSettings && !activeWindows[2].showed) {
		activeWindows[2].set_hidden(false);
	}
	if (g_showWhiteboard && !activeWindows[3].showed) {
		activeWindows[3].set_hidden(false);
	}

	if (g_showWidgetSettings && !activeWindows[4].showed) {
		activeWindows[4].set_hidden(false);
	}

	return true;
}