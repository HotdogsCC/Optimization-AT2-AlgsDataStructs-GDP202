#include "Window.h"
#include <iostream>

namespace aie
{
	Window::Window(uint32_t w, uint32_t h, string title, Color clrColor)
		: m_width{ w }, m_height{ h }, m_title{ std::move(title) },
		m_clearColor{ clrColor }
	{

	}

	Window::~Window()
	{
		CloseWindow();
	}

	bool Window::Init() const
	{
		SetTraceLogLevel(LOG_ERROR);
		//SetConfigFlags(FLAG_VSYNC_HINT);
		InitWindow(static_cast<int>(m_width), static_cast<int>(m_height), m_title.c_str());

		if (IsWindowReady())
		{
			SetTargetFPS(60);
			return true;
		}

		std::cout << "[AIE] Window failed to open!\n";
		return false;
	}

	void Window::BeginFrame()
	{
		BeginDrawing();
		ClearBackground(m_clearColor);
	}

	void Window::EndFrame()
	{
		EndDrawing();
	}

	bool Window::ShouldClose()
	{
		return WindowShouldClose();
	}
}
