#include "Agent.h"
#include <glm/glm.hpp>
#include "../Application.h"
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

#include "../Renderer.h"

namespace aie
{
	// You need to implement a combat system in this function
	// You can add any variables you need to Agent to facilitate this
	void Agent::ResolveCombat(Agent* attacker, Agent* defender, float deltaTime) {
		
	}


	/*
	 * This Tick() function controls the Humans and Zombies
	 * 
	 * Use your collision system to detect if a Human and a Zombie get too close, so they can use the ResolveCombat() function to fight
	 * You must also use it to prevent overlaps, you can do this by preventing an agent from moving if itwould end up overlapping another agent,
	 * or you can push the agents apart gradually so they don't overlap, or any other alternative. The choice is yours.
	 * 
	 */

	void Agent::Tick(float deltaTime) {
		
		// If we're dead don't do anything, just return
		if(!IsAlive())
		{
			std::cout << "dead";
			return;
		}
		
		// Otherwise...
		// Use your range detection system by calling Application::GetApplication()->GetAgentsWithinRange(), to check if another agent is colliding with us
		std::vector<Agent*> agentsWithinRange;
		Application::GetApplication()->GetAgentsWithinRange(agentsWithinRange, position.x, position.y, collisionRange*2);
		
		// If they are, resolve the collision so they don't overlap, or overlapping is minimized
		for (Agent* otherAgent : agentsWithinRange)
		{
			//check that the other agent is not this
			if(otherAgent == this)
			{
				continue;
			}
			//get direction between two agents
			vec2 directionVector = {otherAgent->GetPosition().x - position.x, otherAgent->GetPosition().y - position.y};

			//to stop NaN, make a unit vector if the distance tiny
			if(directionVector.x <= 0.1f)
			{
				directionVector = {0, 1};
			}
			if(directionVector.y <= 0.1f)
			{
				directionVector = {1, 0};
			}
			directionVector = normalize(directionVector);
			
			//get the distance between the two agents
			float distance = glm::distance(position, otherAgent->GetPosition());
			float pushBack = (collisionRange * 2.0f) - distance;
			pushBack /= 2.0f;

			vec2 thisTarget = position;
			vec2 otherTarget = otherAgent->GetPosition();
			
			if(position.x > otherAgent->GetPosition().x)
			{
				thisTarget.x += directionVector.x * pushBack;
				otherTarget.x -= directionVector.x * pushBack;
			}
			else
			{
				thisTarget.x -= directionVector.x * pushBack;
				otherTarget.x += directionVector.x * pushBack;
			}

			if(position.y > otherAgent->GetPosition().y)
			{
				thisTarget.y += directionVector.y * pushBack;
				otherTarget.y -= directionVector.y * pushBack;
			}
			else
			{
				thisTarget.y -= directionVector.y * pushBack;
				otherTarget.y += directionVector.y * pushBack;
			}
			
			SetPosition(thisTarget);
			otherAgent->SetPosition(otherTarget);

			// If the other agent is an 'enemy' ,i.e. their ->faction variables are not the same, use the ResolveCombat() function
			if(otherAgent->GetSide() != faction)
			{
				ResolveCombat(this, otherAgent, deltaTime);
				return;
			}
			
		}
		
		// Use your range detection system to get the closest enemy in range

		
		// 'Target' the closest enemy and set the goal_position equal to the target Agent's position
		
		// Rotate the Agent using the goal_position, position and atan2, to set the 'rotation' variable

		// Get the underlying terrain using Application::GetApplication()->GetMap()->GetWeight(position.x, position.y)
		// to calculate a reduction in speed
		float scaledSpeed = speed - Application::GetApplication()->GetMap()->GetWeight(position.x, position.y);
		
		// Move the Agent towards the goal_position using the speed variable and the reduction from the terrain.
		vec2 directionToGoal = {goal_position.x - position.x, goal_position.y -position.y};
		directionToGoal = normalize(directionToGoal);
		
		vec2 targetPos = position + (directionToGoal * deltaTime * scaledSpeed);
		SetPosition(targetPos);
	}

}








namespace aie
{
	bool Agent::IsAlive() const { return hp > 0; }

	Agent::Agent(Texture2D sp[2], Side f, vec2 pos , float spd, float h) 
		: GameObject{ guid,pos }
		, live_sprite{ sp[0] }
		, dead_sprite{ sp[1] }
		, faction {f}
		, speed{ spd }
		, hp{h}
	{
	}

	void Agent::Render()
	{
		// If there is no sprite, just draw circles
		if(live_sprite.id == 0)
			Renderer::DrawCircle( Circle{ position, size.x }, color);
		else
		{
			// Pick the sprite depending on our state
			const auto sprite = IsAlive() ? live_sprite : dead_sprite;

			// Draw sprite
			DrawTexturePro(
				sprite,
				Rectangle(0.f, 0.f, (float)sprite.width, (float)sprite.height),
				Rectangle(position.x, position.y, size.x, size.y),
				Vector2{ size.x / 2.f, size.y / 2.f },
				rotation,
				RAYWHITE);

			// Draw health bar
			if (Application::GetApplication()->DebugMode() > Application::DebugModes::NONE) {
				DrawLine(
					(int)(position.x - size.x / 2.f),
					(int)(position.y - size.y / 2.f),
					(int)(position.x - size.x / 2.f + hp.Fraction() * size.x),
					(int)(position.y - size.y / 2.f),
					hp.Fraction() > 0.5 ? Color{ 64,255,128,255 } : Color{ 255,64,64,255 }
				);

				//draw collision hitbox
				Renderer::DrawCircleLines({ position, collisionRange }, ORANGE, 1.f);

				//draw visibility hitbox
				Renderer::DrawCircleLines({ position, searchRange }, BLUE, 1.f);
			}
		}

		// If we're dead we're done
		if (!IsAlive())
			return;

		// If we're alive and debug levels are sufficient draw lines to the target
		if(target && Application::GetApplication()->DebugMode()==Application::DebugModes::FULL){
			DrawLine((int)position.x, (int)position.y, (int)target->position.x, (int)target->position.y, LIGHTGRAY);
		}
	}

	void Agent::SetColor(Color c){
		color = c;
	}
	
	void Agent::TakeDamage(float d){
		if (IsAlive() && hp > 0)
			hp -= d;
	}

	const Agent::Side Agent::GetSide()
	{
		return faction;
	}


}


