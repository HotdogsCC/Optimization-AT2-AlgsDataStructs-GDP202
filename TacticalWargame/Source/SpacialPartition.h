#pragma once
#include <vector>
#include <glm/vec2.hpp>
namespace aie
{
    class Agent;

    struct vec2Int
    {
        int x;
        int y;

        bool operator==(const vec2Int& other) const
        {
            return x == other.x && y == other.y;
        }
    };

    class Node
    {
    public:
        Agent* GetPayload();
        void SetPayload(Agent* agent);
        
        Node* GetChild();
        void SetChild(Node* child);
    private:
        //stores the pointer to the value of this node
        Agent* Payload = nullptr;

        //stores the pointer to the next
        Node* Child = nullptr;
    };

    class LinkedList
    {
    public:
        Node* GetHead();
        void PushBack(Agent* agent);
        void Remove(Agent* agent);
    private:
        //the head of the list
        Node* head = nullptr;
    };

    class SpacialPartition
    {
    public:
        //construction
        SpacialPartition();

        //destruction
        ~SpacialPartition();

        //returns a vector with all agents in range of a collision
        void GetAdjacentAgents(float posX, float posY, std::vector<aie::Agent*>& outVector);
        void GetAdjacentAgents(glm::vec2 pos, std::vector<aie::Agent*>& outVector);

        //returns a vector with all agents in the same tile
        void GetAgentsInPartition(float posX, float posY, std::vector<aie::Agent*>& outVector);
        void GetAgentsInPartition(glm::vec2 pos, std::vector<aie::Agent*>& outVector);
        void GetAgentsInPartitionFromCoord(vec2Int coords, std::vector<aie::Agent*>& outVector);

        //returns a vector containing the tile coordinates the position is located in
        vec2Int GetIndex(glm::vec2 pos);
        vec2Int GetIndex(float x, float y) const;

        //adds an agent to a partition
        void AddAgent(aie::Agent* agent);

        //updates which partition the agent is in, removing it from the old one
        void UpdateAgent(aie::Agent* agent);
    private:
        //stores agents that are present in each tile of the partition
        LinkedList*** tiles; 

        //stores the grid size
        int gridSize = 25;
    };
    
    
}


