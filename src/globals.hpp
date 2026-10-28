#pragma once
#include <atomic>
#include <mutex>
#include <vector>

struct MediaInfo {
	std::string title = "Unknown";
	std::string artist = "Unknown";
	bool is_playing = false;
	bool thumbnail_changed = true;
	int time_total = 1;
	int time_current = 1;
	std::vector<uint8_t> image_bytes;
};

inline std::atomic<bool> g_running = true;
inline std::mutex mtx_media;
inline MediaInfo g_mediaInfo;
inline unsigned int g_loadedTextureID = 0;