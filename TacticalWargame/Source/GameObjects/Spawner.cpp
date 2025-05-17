#include "Spawner.h"

#include <corecrt_math.h>
#include <raylib/raylib.h>
#include <stdexcept>

#include "Agent.h"
#include "../Application.h"
#include "../Renderer.h"
#include "../UtilityFunctions.h"

namespace aie
{
	Spawner::Spawner(const glm::vec2& loc, std::shared_ptr<Agent> proto)
		: GameObject{ Spawner::guid }
		, prototype { proto }, spawnRadius{ 50 } 
	{
		position = loc;
		if (!proto)
			throw std::runtime_error("No prototype defined for Spawner");
	}

	Agent* Spawner::Spawn() const
	{
		Agent* newAgent = new Agent{ *prototype };

		const float r = RandomFloat(1.f, spawnRadius);
		const float theta = 2.f * PI * RandomFloat(0.f, 1.f);

		newAgent->SetPosition(
		{
			position.x + r * cosf(theta),
			position.y + r * sinf(theta)
		});

		float scale = RandomFloat(35.f, 50.f);
		newAgent->SetSize(vec2(1.f) * scale);
		newAgent->SetRotation(RandomFloat(0.f, 360.f));

		newAgent->SetColor(ColorFromHSV(RandomFloat(0, 360.f), 1.f, 1.f));

		return newAgent;
	}

    void Spawner::Tick(float dt) {
        spawncounter -= dt;
        while (spawncounter < 0) {
            spawncounter += spawntimer;
            for (int i = 0; i < spawnnumber; ++i) {
                Application::GetApplication()->AddAgent(Spawn());
            }
        }
	 }

	void Spawner::Render()
	{
		if (Application::GetApplication()->DebugMode() > Application::DebugModes::NONE) {
			Renderer::DrawCircleLines({ position, spawnRadius }, RED, 1.f);
		}
	}
}
