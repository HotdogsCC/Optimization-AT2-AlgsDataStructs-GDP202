#pragma once

#include <string>

#include <raylib/raylib.h>

using std::string;

namespace aie
{
	class Window
	{
		friend class Application;

	private:
		uint32_t m_width;
		uint32_t m_height;
		string m_title;
		Color m_clearColor;

	private:
		Window(uint32_t w, uint32_t h, string title, Color clrColor);
		~Window();

	private:
		bool Init() const;

		void BeginFrame();
		void EndFrame();

		bool ShouldClose();

	};
}