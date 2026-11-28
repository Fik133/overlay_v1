#include "topbar.h"
#include "cpu_usage_monitor.h"
#include "../utils.h"
#include <format>
//#TODO Add modal for widget properties.
//#TODO Add settings.


CPUUsageMonitor cpuMonitor;
float cpuTimer = 0.0f;
double cachedCpuUsage = 0.0;
void topbar_metrics();

void tab_buttons() {
	if (ui::topbar::TabButton("Whiteboard", ICON_FA_CHALKBOARD)) {

	}
	ui::topbar::ModalOpenButton("Widgets", ICON_FA_GRIP, "modal-settings-widget", ui::topbar::content::modal::ContentWidgetsSettings);
	ui::topbar::ModalOpenButton("Settings", ICON_FA_GEARS, "modal-settings", ui::topbar::content::modal::ContentSettings);
	if(ui::topbar::TabButton("Browser", ICON_FA_MAGNIFYING_GLASS) ){

	}
}

void ui::windows::topbar()
{

	ImGui::BeginChild("TOPBAR", ImVec2(0,0), ImGuiChildFlags_AutoResizeY);
	
	tab_buttons();

	topbar_metrics();

	ImGui::EndChild();
}

void topbar_metrics() {
	constexpr const char* text_placeholder = ICON_FA_GAUGE " FPS: 000.00";
	float placeholder_size = ImGui::CalcTextSize(text_placeholder).x;

	float value = 1.f / ImGui::GetIO().DeltaTime;
	std::string formatted_value = std::format("{:.2f}", value);
	formatted_value = std::format("{} FPS: {}", ICON_FA_GAUGE, formatted_value);

	ui::tools::TextAlignRight(formatted_value.c_str(), placeholder_size);
	ImGui::SameLine();

	constexpr const char* text_placeholder2 = ICON_FA_MEMORY " RAM: 00.00 / 00.00 GB (00%)";
	float placeholder_size2 = ImGui::CalcTextSize(text_placeholder2).x;

	MemoryInfo mem = GetSystemMemoryUsage();

	formatted_value = std::format("{0:.2f} / {1:.2f} GB ({2}%)", 
		mem.usedGB, mem.totalGB, mem.memoryLoadPercent > 99.f ? 99.f : mem.memoryLoadPercent);

	formatted_value = std::format("{} RAM: {}", ICON_FA_MEMORY, formatted_value);

	ui::tools::TextAlignRight(formatted_value.c_str(), placeholder_size2, placeholder_size);
	ImGui::SameLine();

	constexpr const char* text_placeholder3 = ICON_FA_MICROCHIP " CPU: 000.00%";
	float placeholder_size3 = ImGui::CalcTextSize(text_placeholder3).x;
	formatted_value = std::format("{:.2f}%", cachedCpuUsage);

	formatted_value = std::format("{} CPU: {}", ICON_FA_MICROCHIP, formatted_value);

	ui::tools::TextAlignRight(formatted_value.c_str(), placeholder_size3, placeholder_size+placeholder_size2);

	cpuTimer += ImGui::GetIO().DeltaTime;
	if (cpuTimer >= 1.0f) {
		cachedCpuUsage = cpuMonitor.GetCPUUsagePercent();
		cpuTimer = 0.0f;
	}

}
