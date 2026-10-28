#include <iostream>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <thread>
#include "fonts/ElmsSans.h"
#include "globals.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../external/include/stb_image.h"
#include "ui.hpp"

#ifdef _WIN32
#include <Windows.h>
	constexpr int disable_window = VK_END;
#else
	constexpr int disable_window = GLFW_KEY_END;
#endif // _WIN32

	int passthrough_key = VK_F2;
	bool is_passtrough = true;

extern bool RenderUI();
extern void ThreadMediaWindows();

//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

int main() {
	
	if (!glfwInit()) {
		std::cerr << "[GLFW] Failed to initialize glfw!\n";
		return -1;
	}
	
	const auto monitor = glfwGetPrimaryMonitor();

	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_FLOATING , GLFW_TRUE); //TOPMOST FLAG
	glfwWindowHint(GLFW_MOUSE_PASSTHROUGH  , GLFW_TRUE); //TOPMOST FLAG

	
#ifdef _WIN32
	//Basically instead of making a window of the exact size of the monitor (which causes it to be black because windows sees it as a popup)
	//you make it slightly larger, but then only draw to the part which overlaps
	//your monitor. This ‘fools’ the driver into not engaging
	//the fullscreen path and you can get a transparent overlay over the monitor. 
	int window_width = mode->width + 1; //
#else
	int window_width = mode->width;
#endif // _WIN32


	GLFWwindow* window = glfwCreateWindow(window_width, mode->height, "Overlay", NULL, NULL);

	if (window == NULL) {
		std::cerr << "[GLFW] Failed to initialize window!\n";
		glfwTerminate(); // Cleanup
		return -1;
	}

	glfwSetWindowPos(window, 0, 0);

	std::cout << "[GLFW] Successfully created glfw window\n";

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // enable vsync

	int framebufferWidth, framebufferHeight;
	glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
	glViewport(0, 0, framebufferWidth, framebufferHeight);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  

	ImGuiStyle& style = ImGui::GetStyle();
	styles::ui::dark_color();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	ImFontConfig fontConfig;
	fontConfig.FontDataOwnedByAtlas = false;
	fontConfig.OversampleH = 2;
	fontConfig.OversampleV = 2;

	static const ImWchar ranges[] = {
		0x0020, 0x00FF,
		0,
	};

	ImFont* ElmsSans = io.Fonts->AddFontFromMemoryTTF(ElmsSans_rawData, sizeof(ElmsSans_rawData), 15.0f, &fontConfig, ranges);

	// Creating Media Thread
	std::thread mediaThread(ThreadMediaWindows);
	mediaThread.detach();

	while (!glfwWindowShouldClose(window)) {



#ifdef _WIN32
		if (GetAsyncKeyState(disable_window) & 0x8000) {
			glfwSetWindowShouldClose(window, true);
		}
#else
		if (glfwGetKey(window, disable_window) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}
#endif // _WIN32

#ifdef _WIN32
		if (GetAsyncKeyState(passthrough_key) & 1) {
			is_passtrough = !is_passtrough;
			glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, is_passtrough ? GLFW_TRUE : GLFW_FALSE);
		}
#endif

		glClearColor(0.f, 0.f, 0.f, is_passtrough ? 0.0f : 0.2f);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		RenderUI();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	g_running = false;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();


	return 0;
}
