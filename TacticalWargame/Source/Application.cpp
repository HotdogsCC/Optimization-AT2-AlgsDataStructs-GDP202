#include "Application.h"
#include <glm/glm.hpp>

#include <cstdlib>
#include <stdexcept>
#include <iostream>


#include "Window.h"
#include "GameObjects/Agent.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/Spawner.h"

namespace aie {

	/* You will need to change this function to use your optimised system for finding close-by agents
	 * Change any other source files you need to facilitate this system
	 * 
	 * This function should fill out the 'agents' vector with all those Agents that fall within 'range' distance
	 * of the point defined by (x,y)
	 * 
	 */

	SpacialPartition* Application::GetPartition()
	{
		return m_partition;
	}

	
	void Application::GetAgentsWithinRange(std::vector<Agent*>& agents, float x, float y, float r)
	{
		//if range isn't overloaded, return all agents
		if(r < 0.0f)
		{
			for (GameObject* gameObject : m_objects)
			{
				//try to cast the game object to an agent
				if(Agent* agent = dynamic_cast<Agent*>(gameObject))
				{
					//add the agent to the return vector
					agents.push_back(agent);
				}
			}

			return;
		}

		//get range squared
		const float rangeSqr = r * r;

		//convert floats to vec for clarity
		const vec2 thisPos = {x, y};
		
		//iterate through all agents in scene
		for (GameObject* gameObject : m_objects)
		{
			//try to cast the game object to an agent
			if(Agent* agent = dynamic_cast<Agent*>(gameObject))
			{
				const vec2 agentPos = agent->GetPosition();
				const vec2 resultantVec = {thisPos.x - agentPos.x, thisPos.y - agentPos.y};

				//get the squared distance between input position and agent position
				const float distanceSqr =
					(resultantVec.x * resultantVec.x) + (resultantVec.y * resultantVec.y);

				//check if distance is in range
				if(distanceSqr <= rangeSqr)
				{
					//add agent to return
					agents.push_back(agent);
				}
			}
		}

		

		
	}

}




// You may change any other parts of this class you need to support your system.

namespace aie
{
	Application* Application::m_instance = nullptr;
	SpacialPartition* Application::m_partition = nullptr;

	int Application::Execute()
	{
		// Singleton pattern
		if (m_instance == nullptr)
		{
			m_instance = new Application;

			const int status = m_instance->Run();

			delete m_instance;

			return status;
		}

		return EXIT_FAILURE;
	}

	// Singleton pattern
	Application* Application::GetApplication() {
		return m_instance;
	}

	const Tilemap* Application::GetMap() const {
		return m_instance->map.get();
	}

	float Application::ScreenWidth()
	{
		return static_cast<float>(m_instance->m_window->m_width);
	}

	float Application::ScreenHeight()
	{
		return static_cast<float>(m_instance->m_window->m_height);
	}

	void Application::Quit()
	{
		m_instance->m_shouldQuit = true;
	}

	Application::Application()
		: m_window{ new Window{ 736, 736, "AIE - Tactical WarGame", RAYWHITE } },
		m_shouldQuit{ false }
	{
	}

	Application::~Application()
	{
		delete m_window;
	}

	

	void Application::AddAgent(Agent* agent)
	{
		pending_add.push_back(agent);
	}

	Application::DebugModes Application::DebugMode() const
	{
		return DBMode;
	}

	int Application::Run()
	{
		if (!m_window->Init())
		{
			return EXIT_FAILURE;
		}
		try {
			BeginPlay();

			while (!m_window->ShouldClose() && !m_shouldQuit)
			{
				const auto dt = GetFrameTime();
				Tick(dt);
				Input(dt);

				m_window->BeginFrame();

				Render();

				m_window->EndFrame();
			}

			EndPlay();
		} catch (std::exception& e){
			std::cout << e.what() << std::endl;
			return EXIT_FAILURE;
		}

		return EXIT_SUCCESS;
	}

	void Application::BeginPlay()
	{
		//initalise the output string stream
		outputString << "Collision Resoultion Time (ms)" << std::endl;

		//create a spacial partition index
		if(m_partition == nullptr)
		{
			m_partition = new SpacialPartition();
		}
		
		// Set up the map
		// This loads a texture and a series of weights that map to features in the terrain
		// You can use these weights in the Agent Tick() to control the speed.
		float* data = new float[] { 
			4, 4, 4, 2, 1, 0, 1, 3, 4, 4, 4, 4, 4, 4,
			4, 1, 0, 0, 0, 0, 0, 3, 4, 4, 4, 4, 4, 4,
			4, 2, 2, 0, 0, 0, 0, 0, 0, 2, 2, 3, 4, 4,
			4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 4,
			4, 4, 4, 2, 1, 0, 0, 0, 0, 0, 0, 0, 1, 4,
			4, 4, 4, 4, 3, 1, 0, 0, 0, 0, 0, 0, 0, 4,
			4, 3, 2, 4, 3, 1, 0, 0, 0, 0, 0, 0, 0, 3,
			4, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
			4, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
			4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3,
			4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 0, 0, 3, 4,
			4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 4, 4,
			2, 0, 2, 4, 4, 4, 4, 4, 4, 4, 4, 0, 2, 4,
			4, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 };

		auto maptexture = LoadTexture("Content/map.png");
		if (maptexture.id == 0)
			throw std::runtime_error("Map texture not found");

		map.reset(new Tilemap{ 14,14,ScreenWidth(),ScreenHeight(),data, LoadTexture("Content/map.png") });



		//Set up agents and spawners
		//Soldier spawner
		{
			auto sprite = LoadTexture("Content/survivor.png");
			if (sprite.id == 0)
				throw std::runtime_error("Agent texture not found");
			
			auto deadsprite = LoadTexture("Content/body.png");
			if (deadsprite.id == 0)
				throw std::runtime_error("Body texture not found");

			Texture2D sprites[] = { sprite,deadsprite };
			
			auto agent = std::make_shared<Agent>(sprites, Agent::Side::HUMAN, glm::vec2{},6.0f,10.f);

			const vec2 pos = { ScreenWidth() * 0.5f, ScreenHeight() * 0.5f };
			Spawner* spawner = new Spawner{ pos, agent };

			spawner->spawnnumber = 5;
			spawner->spawnRadius = 150.f;

			m_objects.emplace_back(spawner);
			
		}



		//Zombie spawner
		{
			auto sprite = LoadTexture("Content/zombie.png");
			if (sprite.id == 0)
				throw std::runtime_error("Zombie texture not found");

			auto deadsprite = LoadTexture("Content/body.png");
			if (deadsprite.id == 0)
				throw std::runtime_error("Body texture not found");

			Texture2D sprites[] = { sprite,deadsprite };
			auto agent = std::make_shared<Agent>(sprites,Agent::Side::ZOMBIE,glm::vec2{},5.f, 1.f);

			float vals[] = { 0.15f, 0.85f };

			for (auto v : vals) {
				const glm::vec2 pos = { ScreenWidth() * v, ScreenHeight() * 0.85f };
				Spawner* spawner = new Spawner(pos, agent);

				spawner->spawnnumber = 15;
				spawner->spawnRadius = 150.f;

				m_objects.emplace_back(spawner);
			}
		}

		//Kick start everything
		for(auto& o:m_objects){
			o->BeginPlay();
		}
	}

	void Application::Tick(const float dt)
	{
		//tick each game object
		for(const auto& object : m_objects)
		{
			object->Tick(dt);
		}

		//Add pending objects
		if (!pending_add.empty()) {
			m_objects.insert(m_objects.end(), pending_add.begin(), pending_add.end());
			pending_add.clear();
		}
	}

	void Application::Input(float dt)
	{
		if(IsKeyPressed(KEY_SPACE)){
			switch(DBMode){
			case DebugModes::NONE:
				DBMode = DebugModes::HP_ONLY; break;
			case DebugModes::HP_ONLY:
				DBMode = DebugModes::FULL; break;
			case DebugModes::FULL:
				DBMode = DebugModes::NONE; break;
			}
		}
	}

	void Application::Render() const
	{
		// Render the map first
		map->Render();

		//alive agents should render on top of dead ones

		//stores references to all agents that are alive
		std::vector<Agent*> aliveAgents;

		// Render the objects
		for(const auto& object : m_objects)
		{
			//try to cast to an agent
			if (Agent* agent = dynamic_cast<Agent*>(object))
			{
				//if the agent is alive, save it for the second render pass
				if (agent->IsAlive())
				{
					aliveAgents.push_back(agent);
					continue;
				}
			}
			//if the agent is dead, or the object isn't an agent at all, render it
			object->Render();
			
		}

		// Render the agents that are alive
		for (Agent* agent : aliveAgents)
		{
			agent->Render();
		}
	}

	void Application::EndPlay()
	{
		//output fps data
		std::ofstream output{ "regular.csv" };
		output << outputString.str();
		output.close();

		for(const auto& object : m_objects)
		{
			object->EndPlay();
			delete object;
		}

		m_objects.clear();

		//remove the spacial partition 
		delete m_partition;
	}
}
