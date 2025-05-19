#include "Agent.h"
#include <glm/glm.hpp>
#include "../Application.h"
#include <iostream>
#include <chrono>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

#include "../Renderer.h"
#include "../SpacialPartition.h"

namespace aie
{
	// You need to implement a combat system in this function
	// You can add any variables you need to Agent to facilitate this
	void Agent::ResolveCombat(Agent* attacker, Agent* defender, float deltaTime) {
		//instant resolution

		if(attacker == nullptr || defender == nullptr)
		{
			return;
		}
		
		//remove targets for both
		attacker->target = nullptr;
		defender->target = nullptr;

		
		float damageDealt = 0;
		//find out who has the least HP
		if(attacker->GetHP() > defender->GetHP())
		{
			damageDealt = defender->GetHP();
		}
		else
		{
			damageDealt = attacker->GetHP();
		}
		attacker->TakeDamage(damageDealt);
		defender->TakeDamage(damageDealt);
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
			return;
		}

		// Otherwise...
		// Use your range detection system by calling Application::GetApplication()->GetAgentsWithinRange(), to check if another agent is colliding with us
		// ^^^ this is suboptimal, and a spatial partition will be used instead ^^^

		//get the current time
		auto then = std::chrono::steady_clock::now();

		//	CHANGE FOR TESTING
		//	CHANGE FOR TESTING
		bool usingSpatialParts = true;
		//	CHANGE FOR TESTING
		//	CHANGE FOR TESTING


		/* when using spacial partitions, the vector returned will contain
		*  agents that [might] be colliding. the standard distance check against
		*  all agents will return a vector with agents that [are] colliding
		*/

		std::vector<Agent*> agentsWithinRange;
		if(usingSpatialParts)
		{ 
			//get agents that could be colliding from partition
			Application::GetPartition()->GetAdjacentAgents(GetPosition(), agentsWithinRange);

			// If they are, resolve the collision, so they don't overlap, or overlapping is minimized
			for (Agent* otherAgent : agentsWithinRange)
			{

				//if the other agent is this, or dead, or doesn't exist, we dont care
				if (otherAgent == this || !otherAgent->IsAlive() || otherAgent == nullptr)
				{
					continue;
				}

				//get distance between agents
				vec2 resultantVector = {
					otherAgent->GetPosition().x - GetPosition().x,
					otherAgent->GetPosition().y - GetPosition().y
				};
				float distanceSquared =
					resultantVector.x * resultantVector.x
					+
					resultantVector.y * resultantVector.y;

				//if they aren't colliding, move on to the next
				if (distanceSquared >= (collisionRange * 2 * collisionRange * 2))
				{
					continue;
				}

				//Otherwise, if they are colliding....

				// If the other agent is an 'enemy' ,i.e. their ->faction variables are not the same, use the ResolveCombat() function
				if (otherAgent->GetSide() != faction)
				{
					ResolveCombat(this, otherAgent, deltaTime);
					return;
				}

				//otherwise they are of the same faction

				//get direction between two agents
				vec2 directionVector = { otherAgent->GetPosition().x - position.x, otherAgent->GetPosition().y - position.y };

				//to stop NaN, make a unit vector if the distance tiny
				if (directionVector.x <= 0.1f)
				{
					directionVector = { 0, 1 };
				}
				if (directionVector.y <= 0.1f)
				{
					directionVector = { 1, 0 };
				}
				directionVector = normalize(directionVector);

				//get the distance between the two agents
				float distance = glm::distance(position, otherAgent->GetPosition());
				float pushBack = (collisionRange * 2.0f) - distance;
				pushBack /= 2.0f;

				vec2 thisTarget = position;
				vec2 otherTarget = otherAgent->GetPosition();

				if (position.x > otherAgent->GetPosition().x)
				{
					thisTarget.x += directionVector.x * pushBack;
					otherTarget.x -= directionVector.x * pushBack;
				}
				else
				{
					thisTarget.x -= directionVector.x * pushBack;
					otherTarget.x += directionVector.x * pushBack;
				}

				if (position.y > otherAgent->GetPosition().y)
				{
					thisTarget.y += directionVector.y * pushBack;
					otherTarget.y -= directionVector.y * pushBack;
				}
				else
				{
					thisTarget.y -= directionVector.y * pushBack;
					otherTarget.y += directionVector.y * pushBack;
				}

				//resolve the collision by setting new positions
				SetPosition(thisTarget);
				otherAgent->SetPosition(otherTarget);

				//and then give them both new goals
				int randomNumberX = std::rand() % 60;
				int randomNumberY = std::rand() % 60;
				goal_position = { 338 + randomNumberX, 338 + randomNumberY };

				randomNumberX = std::rand() % 60;
				randomNumberY = std::rand() % 60;
				otherAgent->goal_position = { 338 + randomNumberX, 338 + randomNumberY };



			}
		}

		else
		{
			//get agents that are collding
			Application::GetApplication()->GetAgentsWithinRange(agentsWithinRange, position.x, position.y, collisionRange * 2);

			// If they are, resolve the collision, so they don't overlap, or overlapping is minimized
			for (Agent* otherAgent : agentsWithinRange)
			{

				//if the other agent is this, or dead, or doesn't exist, we dont care
				if (otherAgent == this || !otherAgent->IsAlive() || otherAgent == nullptr)
				{
					continue;
				}

				// If the other agent is an 'enemy' ,i.e. their ->faction variables are not the same, use the ResolveCombat() function
				if (otherAgent->GetSide() != faction)
				{
					ResolveCombat(this, otherAgent, deltaTime);
					return;
				}

				//otherwise they are of the same faction

				//get direction between two agents
				vec2 directionVector = { otherAgent->GetPosition().x - position.x, otherAgent->GetPosition().y - position.y };

				//to stop NaN, make a unit vector if the distance tiny
				if (directionVector.x <= 0.1f)
				{
					directionVector = { 0, 1 };
				}
				if (directionVector.y <= 0.1f)
				{
					directionVector = { 1, 0 };
				}
				directionVector = normalize(directionVector);

				//get the distance between the two agents
				float distance = glm::distance(position, otherAgent->GetPosition());
				float pushBack = (collisionRange * 2.0f) - distance;
				pushBack /= 2.0f;

				vec2 thisTarget = position;
				vec2 otherTarget = otherAgent->GetPosition();

				if (position.x > otherAgent->GetPosition().x)
				{
					thisTarget.x += directionVector.x * pushBack;
					otherTarget.x -= directionVector.x * pushBack;
				}
				else
				{
					thisTarget.x -= directionVector.x * pushBack;
					otherTarget.x += directionVector.x * pushBack;
				}

				if (position.y > otherAgent->GetPosition().y)
				{
					thisTarget.y += directionVector.y * pushBack;
					otherTarget.y -= directionVector.y * pushBack;
				}
				else
				{
					thisTarget.y -= directionVector.y * pushBack;
					otherTarget.y += directionVector.y * pushBack;
				}

				//resolve the collision by setting new positions
				SetPosition(thisTarget);
				otherAgent->SetPosition(otherTarget);

				//and then give them both new goals
				int randomNumberX = std::rand() % 60;
				int randomNumberY = std::rand() % 60;
				goal_position = { 338 + randomNumberX, 338 + randomNumberY };

				randomNumberX = std::rand() % 60;
				randomNumberY = std::rand() % 60;
				otherAgent->goal_position = { 338 + randomNumberX, 338 + randomNumberY };
			}
		}

		// store the amount of time that passed to resolve collisions
		auto now = std::chrono::steady_clock::now();
		double timeToExecute = std::chrono::duration<double, std::milli>(now - then).count();
		Application::GetApplication()->outputString << timeToExecute << std::endl;
		
		//if the target is dead, stop targetting them
		if (target)
		{
			if (!target->IsAlive())
			{
				target = nullptr;
			}
		}

		//if the agent doesn't have a target, try to find one
		if (target == nullptr)
		{
			// Use your range detection system to get the closest enemy in range
			agentsWithinRange.clear();
			Application::GetApplication()->GetAgentsWithinRange(agentsWithinRange, position.x, position.y, searchRange);



			//loop through enemies to find a target
			for (Agent* otherAgent : agentsWithinRange)
			{
				//if the other agent is this, or its dead, we dont care
				if (otherAgent == this || !otherAgent->IsAlive())
				{
					continue;
				}

				//check that it is an enemy
				if (otherAgent->GetSide() != faction)
				{
					//if we haven't found anything yet, this wins by default
					if (target == nullptr)
					{
						target = otherAgent;
						continue;
					}

					//get distance to iterated agent
					vec2 otherResultantVector = {
						position.x - otherAgent->GetPosition().x,
						position.y - otherAgent->GetPosition().y };
					float distanceToOther = (
						otherResultantVector.x * otherResultantVector.x
						+
						otherResultantVector.y * otherResultantVector.y);

					//get distance to current closest
					vec2 currentClosestResultantVector = {
						position.x - target->GetPosition().x,
						position.y - target->GetPosition().y };
					float distanceToCurrentClosest = (
						currentClosestResultantVector.x * currentClosestResultantVector.x
						+
						currentClosestResultantVector.y * currentClosestResultantVector.y);

					//check which one is closer
					if (distanceToOther < distanceToCurrentClosest)
					{
						target = otherAgent;
					}

				}
			}
		}

		
		// 'Target' the closest enemy and set the goal_position equal to the target Agent's position
		if(target)
		{
			goal_position = target->GetPosition();
		}
		//otherwise just move somewhere around the centre
		else
		{
			// pick a new direction every 500ish frames
			int randomShouldMove = std::rand() % 500;
			if (randomShouldMove == 0)
			{
				//picks a random location around the centre of the map
				int randomNumberX = std::rand() % 60;
				int randomNumberY = std::rand() % 60;
				goal_position = { 338 + randomNumberX, 338 + randomNumberY };
			}
		}
		
		// Rotate the Agent using the goal_position, position and atan2, to set the 'rotation' variable
		rotation = atan2(goal_position.y - position.y, goal_position.x - position.x) * 57.29578f;

		// Get the underlying terrain using Application::GetApplication()->GetMap()->GetWeight(position.x, position.y)
		// to calculate a reduction in speed
		float scaledSpeed = speed - Application::GetApplication()->GetMap()->GetWeight(position.x, position.y);
		
		// Move the Agent towards the goal_position using the speed variable and the reduction from the terrain.
		vec2 directionToGoal = {goal_position.x - position.x, goal_position.y -position.y};
		directionToGoal = normalize(directionToGoal);

		//set the new position to the target
		vec2 targetPos = position + (directionToGoal * deltaTime * scaledSpeed);
		SetPosition(targetPos);

		//check if the agent has entered a new spacial partition zone
		if(currentSpatialIndex != Application::GetPartition()->GetIndex(GetPosition()))
		{
			//update the agent to be apart of the correct spacial partition zone
			Application::GetPartition()->UpdateAgent(this);
			currentSpatialIndex = Application::GetPartition()->GetIndex(GetPosition());
		}
	}

	vec2Int Agent::GetCurrentSpatialIndex()
	{
		return currentSpatialIndex;
	}


}








namespace aie
{
	bool Agent::IsAlive() const {
		return hp > 0;
		}

	Agent::Agent(Texture2D sp[2], Side f, vec2 pos , float spd, float h) 
		: GameObject{ guid,pos }
		, live_sprite{ sp[0] }
		, dead_sprite{ sp[1] }
		, faction {f}
		, speed{ spd }
		, hp{h}
	{
		//find out where the agent is and initialise with the partition
		currentSpatialIndex = Application::GetPartition()->GetIndex(GetPosition());
		Application::GetPartition()->AddAgent(this);
		
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


