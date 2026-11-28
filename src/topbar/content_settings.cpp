#pragma once
#include "topbar.h"
#include "../utils.h"
inline const std::vector<TabData> setting_tabs = {
	{ SettingTab::Accounts,        "Accounts",        ICON_FA_CIRCLE_USER,          ui::topbar::content::modal::ContentAccounts },
	{ SettingTab::Accessibility,   "Accessibility",   ICON_FA_UNIVERSAL_ACCESS, ui::topbar::content::modal::ContentAccessibility },
	{ SettingTab::Personalization, "Personalization", ICON_FA_PALETTE,          ui::topbar::content::modal::ContentPersonalization },
	{ SettingTab::Advanced,        "Advanced",        ICON_FA_WRENCH,          ui::topbar::content::modal::ContentAdvanced },
};

TabData currentTab = setting_tabs[0]; // by default first tab

void ui::topbar::content::modal::ContentSettings()
{
	ImGui::Text("Settings");

	float button_width = ImGui::GetWindowWidth()/3;

	for (const auto& btn : setting_tabs)
	{
		if (ui::topbar::modal::SettingButton(btn.text, btn.icon, button_width)) {
			currentTab = btn;
		}
	}

	{
		float cp = ImGui::GetCursorPosY();

		float availY = ImGui::GetContentRegionAvail().y;

		float expected_height = ImGui::GetFrameHeight();

		ImGui::SetCursorPosY(availY + cp - expected_height);
	}


	if (ui::topbar::modal::SettingButton("Close", ICON_FA_XMARK, button_width)) {
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

void ui::topbar::content::modal::ContentAccounts()
{
	ImGui::Text("%s", __FUNCSIG__);
}

void ui::topbar::content::modal::ContentAccessibility()
{
	ImGui::Text("%s", __FUNCSIG__);
}

void ui::topbar::content::modal::ContentPersonalization()
{
	ImGui::Text("%s", __FUNCSIG__);
}

void ui::topbar::content::modal::ContentAdvanced()
{
	ImGui::Text("%s", __FUNCSIG__);
}
