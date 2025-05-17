#pragma once

#include "GameObject.h"
#include <memory>

namespace aie
{
	class Agent;

	class Spawner : public GameObject
	{
	public:
		static const GUID guid = 0x5A8A6A2E;

		//Prototype pattern, multiple spawners can share one agent
		std::shared_ptr<Agent> prototype;
		float spawncounter = 0.0f;
	public:
		float spawnRadius;

	public:
		//How long between spawns
		float spawntimer = 200.f;

		//Number of agents to spawn at a time
		int spawnnumber = 1;

		// Spawner takes ownership of the proto
		Spawner(const glm::vec2& loc, std::shared_ptr<Agent> proto);

		// Create an agent
		Agent* Spawn() const;

		void Tick(float dt);
		void Render() override;

	};
}