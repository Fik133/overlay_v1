#pragma once
#include "../ui.hpp"
#include <imgui.h>

namespace ui::topbar {
	bool TabButton(const char* text, const char* icon, bool sameline = true, ImVec2 size = ImVec2(0,0), std::function<void()> event = nullptr);
	bool ModalOpenButton(const char* text, const char* icon, const char* modal, FContent content);
	namespace modal {
		bool SettingButton(const char* text, const char* icon, float width);
		void HandleModal(const char* modal, FContent content);
	}
	namespace content {
		namespace modal {
			void ContentSettings();
			void ContentAccounts();
			void ContentAccessibility();
			void ContentPersonalization();
			void ContentAdvanced();
			void ContentWidgetsSettings();
		}
	}
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