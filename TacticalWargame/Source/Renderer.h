#pragma once

#include <glm/vec2.hpp>
#include <raylib/raylib.h>

using glm::vec2;

namespace aie
{
	struct Rect
	{
	public:
		vec2 center;
		vec2 size;

	public:
		Rect(vec2 center, vec2 size);
		Rect(float x, float y, float w, float h);

	};

	struct Circle
	{
	public:
		vec2 center;
		float radius;

	public:
		Circle(vec2 center, float radius);
		Circle(float x, float y, float radius);

	};

	class Renderer
	{
	public:
		static void DrawCircle(Circle circle, Color color);
		static void DrawCircleLines(Circle circle, Color color, float thickness = 1.f);
		
		static void DrawRectangle(Rect rect, Color color, float rotation = 0.f);
		static void DrawRectangleLines(Rect rect, Color color, float thickness = 1.f, float rotation = 0.f);

		static void DrawLine(vec2 start, vec2 end, Color color, float thickness = 1.f);

		static void DrawTexture(Texture2D texture, vec2 pos, Color tint, vec2 origin = { 0.f, 0.f });
		static void DrawTexture(Texture2D texture, Rect src, Rect dst, Color tint, float rotation = 0.f, vec2 origin = { 0.f, 0.f });

	private:
		static Vector2 To(vec2 vec);
		static Rectangle To(Rect rect);

	};
}