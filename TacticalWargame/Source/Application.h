#pragma once

#include <vector>
#include "GameObjects/Tilemap.h"
#include "GameObjects/Agent.h"

using std::vector;

namespace aie
{
	class Window;
	class GameObject;

	class Application
	{
	public:
		static int Execute();
		static Application* GetApplication();
		static float ScreenWidth();
		static float ScreenHeight();

		static void Quit();

		const Tilemap* GetMap() const;

		enum class DebugModes {
			NONE,
			HP_ONLY,
			FULL
		};

	private:
		static Application* m_instance;

		std::unique_ptr<Tilemap> map;

	public:
		// Add what you need to make this more efficient.
		// Get the agents close enough, then loop through to limit them by side etc.
		void GetAgentsWithinRange(std::vector<Agent*>& agents, float x, float y, float r = -1 );
		void AddAgent(Agent*);

		DebugModes DebugMode() const;

	private:
		DebugModes DBMode = DebugModes::HP_ONLY;
		
		Window* m_window;

		bool m_shouldQuit;

		//List of 
		vector<GameObject*> m_objects;
		vector<GameObject*> pending_add;

	private:
		Application();
		~Application();

	private:
		int Run();

		void BeginPlay();

		void Tick(float dt);
		void Input(float dt);
		void Render() const;

		void EndPlay();

		
	};
}