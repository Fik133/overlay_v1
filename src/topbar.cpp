
#include "ui.hpp"
#include <imgui.h>
#include "../external/include/IconsFontAwesome7.h"
#include <string>
#include <Windows.h>
#include <format>

bool tb_button(const char* text, const char* icon) {
	std::string button_text = std::string(icon) + " " + std::string(text);
	bool clicked = ImGui::Button(button_text.c_str());

	if (ImGui::IsItemHovered()) {
		ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
	}

	ImGui::SameLine();

	return clicked;
}

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

void ui::windows::topbar()
{

	ImGui::BeginChild("TOPBAR", ImVec2(0,0), ImGuiChildFlags_AutoResizeY);
	
	tb_button("Whiteboard", ICON_FA_CHALKBOARD);
	tb_button("Widgets", ICON_FA_GRIP);
	tb_button("Settings", ICON_FA_GEARS);

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
