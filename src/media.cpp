#include "ui.hpp"
#include <string>
// windows

#ifdef _WIN32

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Media.Control.h>
namespace SMTC = winrt::Windows::Media::Control;
struct SystemMediaInfo {
	std::string title = "Brak mediów";
	std::string artist = "Nieznany";
	bool isPlaying = false;
};

SystemMediaInfo GetWindowsMediaInfo() {
	SystemMediaInfo info;

	try {
		auto taskManager = SMTC::GlobalSystemMediaTransportControlsSessionManager::RequestAsync();
		auto manager = taskManager.get();

		if (!manager) return info;

		auto session = manager.GetCurrentSession();
		if (!session) return info;

		auto playbackInfo = session.GetPlaybackInfo();
		if (playbackInfo && playbackInfo.PlaybackStatus() == SMTC::GlobalSystemMediaTransportControlsSessionPlaybackStatus::Playing)
		{
			info.isPlaying = true;
		}

		auto propsTask = session.TryGetMediaPropertiesAsync();
		auto props = propsTask.get();

		if (props) {
			info.title = winrt::to_string(props.Title());
			info.artist = winrt::to_string(props.Artist());
		}
	}
	catch (...) {

	}
	return info;
}
#endif // _WIN32


void ui::windows::mediaplayer() {



	ImGui::InvisibleButton("album_placeholder", ImVec2(64, 64));

	ImVec2 pMin = ImGui::GetItemRectMin();
	ImVec2 pMax = ImGui::GetItemRectMax();

	ImGui::GetWindowDrawList()->AddRectFilled(
		pMin,
		pMax,
		IM_COL32(20, 20, 20, 255),
		6.0f
	);

	ImGui::GetWindowDrawList()->AddRect(
		pMin, pMax, 
		IM_COL32(50, 50, 50, 255),           
		6.0f, 0, 1.0f                         
	);

	float cpy = ImGui::GetCursorPosY();

	ImGui::SameLine();
	ImGui::BeginGroup(); 
	{
		ImGui::Text("Tytuł: Cyberpunk OST");

		ImGui::SetCursorPosY(cpy - 30.0f);
		ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "P.T. Adamczyk");
	}
	ImGui::EndGroup();
}