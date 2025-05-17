#pragma once

#include <raylib/raylib.h>

#include "GameObject.h"
#include "../Attribute.h"

namespace aie
{
	class Agent : public GameObject
	{
	public:
		static const GUID guid = 0xDAA5A8E4;
		enum class Side { HUMAN, ZOMBIE };
		
		Agent(Texture2D sp[2], Side f, vec2 pos = {}, float speed = 5.0f, float hp = 10.f);

		void Render() override;
		void Tick(float dt) override;

		void SetColor(Color c);

		bool IsAlive() const;

		void TakeDamage(float d);

		static void ResolveCombat(Agent* attacker, Agent* defender, float dt);

	private:
		Attribute hp = 10;
		Texture live_sprite;
		Texture dead_sprite;
		Color color = RAYWHITE;
		Agent* target = nullptr;

		// The Agent always moves straight towards the goal_position
		// Set this to be another Agent posiiton to make it chase the agent
		// Set to an arbitrary position to wander round the map
		glm::vec2 goal_position = { 0,0 };
		Side faction = Side::HUMAN;
		float speed = 5.0f;
	};
}

