
#include "ui.hpp"
#include <imgui.h>
#include "../external/include/IconsFontAwesome7.h"
#include <string>
#include <Windows.h>
#include <format>
#include "globals.hpp"
#include <numbers>

//#TODO Add modal for widget properties.

bool tb_button(const char* text, const char* icon, bool sameline = true, ImVec2 size = ImVec2(0,0), std::function<void()> event = nullptr) {
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

// AI code from here
class CPUUsageMonitor {
	ULARGE_INTEGER lastIdleTime = {};
	ULARGE_INTEGER lastKernelTime = {};
	ULARGE_INTEGER lastUserTime = {};

	ULONGLONG SubtractTimes(ULARGE_INTEGER a, ULARGE_INTEGER b) {
		return (a.QuadPart - b.QuadPart);
	}

public:
	double GetCPUUsagePercent() {
		FILETIME idleTime, kernelTime, userTime;
		if (!GetSystemTimes(&idleTime, &kernelTime, &userTime)) {
			return 0.0;
		}

		ULARGE_INTEGER idle, kernel, user;
		idle.LowPart = idleTime.dwLowDateTime;
		idle.HighPart = idleTime.dwHighDateTime;
		kernel.LowPart = kernelTime.dwLowDateTime;
		kernel.HighPart = kernelTime.dwHighDateTime;
		user.LowPart = userTime.dwLowDateTime;
		user.HighPart = userTime.dwHighDateTime;

		ULONGLONG idleDiff = SubtractTimes(idle, lastIdleTime);
		ULONGLONG kernelDiff = SubtractTimes(kernel, lastKernelTime);
		ULONGLONG userDiff = SubtractTimes(user, lastUserTime);

		lastIdleTime = idle;
		lastKernelTime = kernel;
		lastUserTime = user;

		ULONGLONG totalSys = kernelDiff + userDiff;
		if (totalSys == 0) return 0.0;

		double cpuUsage = static_cast<double>(totalSys - idleDiff) / static_cast<double>(totalSys) * 100.0;
		return cpuUsage;
	}
};

struct MemoryInfo {
	double totalGB;
	double usedGB;
	unsigned long long memoryLoadPercent;
};

MemoryInfo GetSystemMemoryUsage() {
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);

	GlobalMemoryStatusEx(&statex);

	double total = static_cast<double>(statex.ullTotalPhys);
	double available = static_cast<double>(statex.ullAvailPhys);
	double used = total - available;

	const double gbDiv = 1024.0 * 1024.0 * 1024.0;

	return {
		total / gbDiv,
		used / gbDiv,
		statex.dwMemoryLoad 
	};
}
// to here

void align_right(float width)
{
	float target_x = ImGui::GetContentRegionMax().x - width;
	ImGui::SetCursorPosX(target_x);
}

void tb_text_value_right(const char* text, const char* icon,const char* value, float max_width, float offset = 0)
{
	std::string text_value =
		std::string(icon) + " " +
		std::string(text) + ": " +
		std::string(value);

	float oldX = ImGui::GetCursorPosX();

	align_right(max_width + (offset) + (offset == 0 ? 0 : ImGui::GetStyle().ItemSpacing.x));
	ImGui::TextUnformatted(text_value.c_str());

	ImGui::SetCursorPosX(oldX);
	
	ImGui::SameLine();
	ImGui::Dummy(ImVec2(0, 0));
}

CPUUsageMonitor cpuMonitor;
float cpuTimer = 0.0f;
double cachedCpuUsage = 0.0;

void accounts_content() {
	ImGui::Text("hello accounts tab");
}

void accessibility_content() {
	ImGui::Text("hello accessibility_content tab");
}

void personalization_content() {
	ImGui::Text("hello personalization_content tab");
}

void advanced_content() {
	ImGui::Text("hello advanced_content tab");
}

enum class SettingTab {
	Accounts,
	Accessibility,
	Personalization,
	Advanced
};

struct TabData {
	SettingTab id;
	const char* text;
	const char* icon;
	FContent content;
};

const std::vector<TabData> setting_tabs = {
	{ SettingTab::Accounts,        "Accounts",        ICON_FA_CIRCLE_USER,          accounts_content },
	{ SettingTab::Accessibility,   "Accessibility",   ICON_FA_UNIVERSAL_ACCESS, accessibility_content },
	{ SettingTab::Personalization, "Personalization", ICON_FA_PALETTE,          personalization_content },
	{ SettingTab::Advanced,        "Advanced",        ICON_FA_WRENCH,          advanced_content },
};

bool st_button(const char* text, const char* icon, float width) { //st_button = setting button
	bool result = tb_button(text, icon, false, ImVec2(width, 0));

	return result;
}

TabData currentTab = setting_tabs[0]; // by default first tab

void settings_content() {
	ImGui::Text("Settings");
	
	float button_width = ImGui::GetWindowWidth()/3;

	for (const auto& btn : setting_tabs)
	{
		if (st_button(btn.text, btn.icon, button_width)) {
			currentTab = btn;
		}
	}

	{
		float cp = ImGui::GetCursorPosY();

		float availY = ImGui::GetContentRegionAvail().y;

		float expected_height = ImGui::GetFrameHeight();

		ImGui::SetCursorPosY(availY + cp - expected_height);
	}


	if (st_button("Close", ICON_FA_XMARK, button_width)) {
		ImGui::CloseCurrentPopup();
	}

	if (!currentTab.content)
		return;

	float cp = ImGui::GetCursorPosX();
	float x_spacing = ImGui::GetStyle().ItemSpacing.x;
	float y_padding = ImGui::GetStyle().WindowPadding.y;

	ImGui::SetCursorPos(ImVec2(cp + button_width + x_spacing, y_padding));

	currentTab.content();

}

void handle_modal(const char* modal, std::function<void()> content) {
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();

	float window_size = g_FrameBufferSize.x/3;

	ImGui::SetNextWindowSize(ImVec2(window_size, window_size/std::numbers::phi));

	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	ImGuiWindowFlags popup_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar;

	if (ImGui::BeginPopupModal(modal, nullptr, popup_flags)) {
		
		content();
		ImGui::EndPopup();
	}
	
}

void ui::windows::topbar()
{

	ImGui::BeginChild("TOPBAR", ImVec2(0,0), ImGuiChildFlags_AutoResizeY);
	
	if (tb_button("Whiteboard", ICON_FA_CHALKBOARD)) {
		g_showWhiteboard = !g_showWhiteboard;
	}
	if (tb_button("Widgets", ICON_FA_GRIP)) {
		g_showWidgetSettings = !g_showWidgetSettings;
	};
	if(tb_button("Settings", ICON_FA_GEARS) ){
		ImGui::OpenPopup("modal-settings");
	}
	
	handle_modal("modal-settings", settings_content);

	constexpr const char* text_placeholder = ICON_FA_GAUGE " FPS: 000.00";
	float placeholder_size = ImGui::CalcTextSize(text_placeholder).x;

	float value = 1.f / ImGui::GetIO().DeltaTime;
	std::string formatted_value = std::format("{:.2f}", value);
	tb_text_value_right("FPS", ICON_FA_GAUGE, formatted_value.c_str(), placeholder_size);
	ImGui::SameLine();

	constexpr const char* text_placeholder2 = ICON_FA_MEMORY " RAM: 00.00 / 00.00 GB (00%)";
	float placeholder_size2 = ImGui::CalcTextSize(text_placeholder2).x;

	MemoryInfo mem = GetSystemMemoryUsage();

	formatted_value = std::format("{0:.2f} / {1:.2f} GB ({2}%)", 
		mem.usedGB, mem.totalGB, mem.memoryLoadPercent > 99.f ? 99.f : mem.memoryLoadPercent);

	tb_text_value_right("RAM", ICON_FA_MEMORY, formatted_value.c_str(), placeholder_size2, placeholder_size);
	ImGui::SameLine();

	constexpr const char* text_placeholder3 = ICON_FA_MICROCHIP " CPU: 000.00%";
	float placeholder_size3 = ImGui::CalcTextSize(text_placeholder3).x;
	formatted_value = std::format("{:.2f}%", cachedCpuUsage);
	tb_text_value_right("CPU", ICON_FA_MICROCHIP, formatted_value.c_str(), placeholder_size3, placeholder_size+placeholder_size2);

	cpuTimer += ImGui::GetIO().DeltaTime;
	if (cpuTimer >= 1.0f) {
		cachedCpuUsage = cpuMonitor.GetCPUUsagePercent();
		cpuTimer = 0.0f;
	}

	ImGui::EndChild();
}
