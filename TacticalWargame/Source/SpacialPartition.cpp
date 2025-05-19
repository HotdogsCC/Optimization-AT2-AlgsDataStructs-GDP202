#include "SpacialPartition.h"

#include "Application.h"

//construction 
aie::SpacialPartition::SpacialPartition()
{
    tiles = new LinkedList** [gridSize];
    //initialise lists
    for(int i = 0; i < gridSize; i++)
    {
        tiles[i] = new LinkedList* [gridSize];
        for(int j = 0; j < gridSize; j++)
        {
            tiles[i][j] = new LinkedList;
        }
    }
}

aie::SpacialPartition::~SpacialPartition()
{
    //initialise lists
    for(int i = 0; i < gridSize; i++)
    {
        for(int j = 0; j < gridSize; j++)
        {
            delete tiles[i][j];
        }
    }
}


//returns a vector with all agents in range of a collision
void aie::SpacialPartition::GetAdjacentAgents(glm::vec2 pos, std::vector<aie::Agent*>& outVector)
{
    GetAdjacentAgents(pos.x, pos.y, outVector);
}

//returns a vector with all agents in range of a collision
void aie::SpacialPartition::GetAdjacentAgents(float posX, float posY, std::vector<aie::Agent*>& outVector)
{
    //get the tile coords given the player position
    vec2Int tilePosition = GetIndex(posX, posY);
    
    //loop through all 9 possible tiles
    for(int i = -1; i < 2; i++)
    {
        for(int j = -1; j < 2; j++)
        {
            //make a temp vec2 that is displaced slightly from the origin
            vec2Int curTilePosition = tilePosition;
            curTilePosition.x += i;
            curTilePosition.y += j;

            //check that the added displacement isn't out of bounds
            if(
                curTilePosition.x < 0 ||
                curTilePosition.y < 0 ||
                curTilePosition.x >= gridSize ||
                curTilePosition.y >= gridSize)
            {
                //iterate to next item if it is out of bounds
                continue;
            }

            //add agents from this partition into the output vector
            GetAgentsInPartitionFromCoord(curTilePosition, outVector);
        }
    }
}

//returns a vector with all agents in the same tile
void aie::SpacialPartition::GetAgentsInPartition(glm::vec2 pos, std::vector<aie::Agent*>& outVector)
{
    GetAgentsInPartition(pos.x, pos.y, outVector);
}

//returns a vector with all agents in the same tile
void aie::SpacialPartition::GetAgentsInPartition(float posX, float posY, std::vector<aie::Agent*>& outVector)
{
    //get the tile coords given the player position
    vec2Int tilePosition = GetIndex(posX, posY);

    GetAgentsInPartitionFromCoord(tilePosition, outVector);
}

//returns a vector with all agents in the same tile
void aie::SpacialPartition::GetAgentsInPartitionFromCoord(vec2Int coords, std::vector<aie::Agent*>& outVector)
{
    //returns all agents in that tile
    Node* node = tiles[coords.x][coords.y]->GetHead();

    //if there is nothing here, return
    if(node == nullptr)
    {
        return;
    }
    //while there are elements down the linked the list
    while(node)
    {
        //add the agent to the output
        outVector.push_back(node->GetPayload());
        //iterate to next node
        node = node->GetChild();
    }
}

aie::vec2Int aie::SpacialPartition::GetIndex(glm::vec2 pos)
{
    return GetIndex(pos.x, pos.y);
}


aie::vec2Int aie::SpacialPartition::GetIndex(float posX, float posY) const
{
    //gets a normalised position (between 0 and 1)
    posX /= aie::Application::GetApplication()->ScreenWidth();
    posY /= aie::Application::GetApplication()->ScreenHeight();

    //gets the tile index of that position 
    int xIndex = static_cast<int>(gridSize * posX);
    int yIndex = static_cast<int>(gridSize * posY);

    //overflow correction
    if (posX >= 1.0f)
        xIndex = gridSize - 1;
    if (posY >= 1.0f)
        yIndex = gridSize - 1;
    if (posX < 0.0f)
        xIndex = 0;
    if (posY < 0.0f)
        yIndex = 0;

    const vec2Int tilePosition = {xIndex, yIndex};
    return tilePosition;
}

void aie::SpacialPartition::AddAgent(aie::Agent* agent)
{
    vec2Int coords = GetIndex(agent->GetPosition());

    //add the agent to the partition
    tiles[coords.x][coords.y]->PushBack(agent);
}

void aie::SpacialPartition::UpdateAgent(aie::Agent* agent)
{
    //get which partition the agent currently is in
    vec2Int coords = agent->GetCurrentSpatialIndex();
    
    //removes from the index it was found at
    tiles[coords.x][coords.y]->Remove(agent);

    //adds the agent to the new partition
    AddAgent(agent);
}

void aie::LinkedList::PushBack(Agent* agent)
{
    //if the head is empty, this is now the start
    if(head == nullptr)
    {
        //assign that node's child to refer to the agent
        Node* newNodeInstance = new Node;
        newNodeInstance->SetPayload(agent);
        head = newNodeInstance;
        return;
    }
    
    Node* node = head;
    Node* parentNode = nullptr;
    //keep exploring down the list until there is a break
    while(node)
    {
        parentNode = node;
        node = node->GetChild();
    }

    //create the node with the payload
    node = new Node;
    node->SetPayload(agent);

    //add it to the child
    parentNode->SetChild(node);
}

void aie::LinkedList::Remove(Agent* agent)
{
    Node* node = head;
    Node* parentNode = nullptr;
    //keep going down the list until the agent is found
    while(node)
    {
        //if we found the agent
        if(node->GetPayload() == agent)
        {
            //if this is at the head
            if(node == head)
            {
                //sets the new head as the child
                head = node->GetChild();
                
            }
            else
            {
                /*otherwise, set the parent of this node's child (which is currently this)
             *to be the child of this node
             */
                parentNode->SetChild(node->GetChild());
            }

            //delete node;
            return;
        }

        //set the parentNode as this node, so the next iteration has the reference to the last
        parentNode = node;
        
        //otherwise move on to next child
        node = node->GetChild();
    }
}

aie::Node* aie::LinkedList::GetHead()
{
    return head;
}

aie::Agent* aie::Node::GetPayload()
{
    return Payload;
}

void aie::Node::SetPayload(Agent* agent)
{
    Payload = agent;
}

aie::Node* aie::Node::GetChild()
{
    return Child;
}

void aie::Node::SetChild(Node* child)
{
    Child = child;
}



