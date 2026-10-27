#pragma once
#include <atomic>
#include <mutex>
#include <vector>

struct MediaInfo {
	std::string title = "Unknown";
	std::string artist = "Unknown";
	bool is_playing = false;
	int time_total = 1;
	int time_current = 1;
};


inline std::atomic<bool> g_running = true;
inline std::mutex mtx_media;
inline MediaInfo g_mediaInfo;