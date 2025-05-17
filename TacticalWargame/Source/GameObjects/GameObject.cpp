#include "GameObject.h"
#include <raylib/raylib.h>
#include <algorithm>

namespace aie
{
	GameObject::GameObject(GUID t, vec2 pos , vec2 siz, float rot )
		: type{ t }, position{ pos }, size{ siz }, rotation{ rot }
	{
	}

	const GUID  GameObject::GetGuid() const { return type; }
	const vec2& GameObject::GetPosition() const { return position; }

	void GameObject::SetPosition(const vec2& p) { position = p; }
	void GameObject::SetRotation(float r) { rotation = r; }
	void GameObject::SetSize(const vec2& s) { size = s; }

	float GameObject::GetRotation() const { return rotation; }
	const vec2& GameObject::GetSize() const { return size; }

	void GameObject::BeginPlay()
	{
	}

	void GameObject::Tick(float dt)
	{
	}

	void GameObject::Render()
	{
		DrawCircle(position.x, position.y, std::max(size.x, size.y), Color{ 0,255,128,64 });
	}

	void GameObject::EndPlay()
	{
	}
}
