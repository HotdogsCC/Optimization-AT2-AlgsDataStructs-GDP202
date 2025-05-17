#include "Renderer.h"

#include <raylib/rlgl.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/rotate_vector.hpp>

using glm::mat4;
using glm::quat;
using glm::vec3;
using glm::vec4;

namespace aie
{
	Rect::Rect(vec2 center, vec2 size)
		: center{ center }, size{ size }
	{
	}

	Rect::Rect(float x, float y, float w, float h)
		: center{ x, y }, size{ w, h }
	{
	}

	Circle::Circle(vec2 center, float radius)
		: center{ center }, radius{ radius }
	{
	}

	Circle::Circle(float x, float y, float radius)
		: center{ x, y }, radius{ radius }
	{
	}

	void Renderer::DrawCircle(Circle circle, Color color)
	{
		DrawCircleV(To(circle.center), circle.radius, color);
	}

	void Renderer::DrawCircleLines(Circle circle, Color color, float thickness)
	{
		constexpr int segments = 36;
		float angleStep = 360.f / segments;

		vec2 offset = { 0, circle.radius };

		DrawLineEx(
			To(circle.center + offset),
			To(circle.center + rotate(offset, glm::radians(angleStep))),
			thickness,
			color);

		for (int i = 0; i < segments; ++i)
		{
			float angleA = angleStep * static_cast<float>(i);
			float angleB = angleStep * static_cast<float>(i + 1);

			DrawLineEx(
				To(circle.center + rotate(offset, glm::radians(angleA))),
				To(circle.center + rotate(offset, glm::radians(angleB))),
				thickness,
				color);
		}
	}

	void Renderer::DrawRectangle(Rect rect, Color color, float rotation)
	{
		::DrawRectanglePro(To(rect), To(rect.size * 0.5f), rotation, color);
	}

	void Renderer::DrawRectangleLines(Rect rect, Color color, float thickness, float rotation)
	{
		const auto [x, y] = To(rect.center);
		const auto [w, h] = To(rect.size);

		rlPushMatrix();

		rlTranslatef(x, y, 0);

		rlRotatef(rotation, 0, 0, -1);

		DrawRectangleLinesEx({ 0.f, 0.f, w, h }, thickness, color);

		rlPopMatrix();
	}

	void Renderer::DrawLine(vec2 start, vec2 end, Color color, float thickness)
	{
		DrawLineEx(To(start), To(end), thickness, color);
	}

	void Renderer::DrawTexture(Texture2D texture, vec2 pos, Color tint, vec2 origin)
	{
		Rect src = { 0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height) };
		Rect dst = { pos, { static_cast<float>(texture.width), static_cast<float>(texture.height) } };

		DrawTexturePro(texture, To(src), To(dst), To(origin), 0.f, tint);
	}

	void Renderer::DrawTexture(Texture2D texture, Rect src, Rect dst, Color tint, float rotation, vec2 origin)
	{
		DrawTexturePro(texture, To(src), To(dst), To(origin), rotation, tint);
	}

	Vector2 Renderer::To(vec2 vec)
	{
		return { vec.x, vec.y };
	}

	Rectangle Renderer::To(Rect rect)
	{
		return { rect.center.x, rect.center.y, rect.size.x, rect.size.y };
	}
}
