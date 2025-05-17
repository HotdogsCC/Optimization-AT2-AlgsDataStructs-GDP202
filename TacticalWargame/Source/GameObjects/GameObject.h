#pragma once

#include <glm/vec2.hpp>

using glm::vec2;

namespace aie
{
	using GUID = unsigned int;

	class GameObject
	{
	protected:
		const GUID type;
		vec2 position;
		vec2 size;
		float rotation;

	public:
		GameObject(GUID t, vec2 pos = {}, vec2 siz = {}, float rot = 0.0f);
		virtual ~GameObject() = default;

		const GUID GetGuid() const;
		const vec2& GetPosition() const;
		float GetRotation() const;
		const vec2& GetSize() const;

		void SetPosition(const vec2& p);
		void SetRotation(float r);
		void SetSize(const vec2& s);

		virtual void BeginPlay();

		virtual void Tick(float dt);
		virtual void Render();

		virtual void EndPlay();

	};
}