#include "MiniginPCH.h"
#include "AIComponent.h"

#include "AStar.h"
#include "Command.h"
#include "GameObject.h"
#include "GridComponent.h"
#include "LevelManager.h"
#include "LocationComponent.h"

dae::AIComponent::AIComponent(std::shared_ptr<GameObject> go) : Component(go)
{
	m_assert(GetEntity()->GetComponent<GridComponent>() != nullptr,
	         "The AIComponent needs a GridComponent to work. Verify that you added one before the AIComponent");
}

void dae::AIComponent::Update(float)
{
	auto& levelManager = LevelManager::GetInstance();
	// TODO Add some caching into the path-finding. This does not need to be done every frame.
	Graph<GraphNode, GraphConnection>& g = levelManager.GetLevelGraph();

	auto astar = AStar(&g, HeuristicFunctions::Manhattan);
	// Get my node in the graph
	const auto gridComp = GetEntity()->GetComponent<GridComponent>();
	const auto gridPos  = gridComp->GetGridPos();
	GraphNode* nodeAI   = g.GetNodeAtWorldPos(gridPos);

	// We have 2 ways to update
	// 1. If we are at the target only to the target if we are fully aligned.
	// This way we complete our movement and are sure to not get stuck halfway a node
	// 2. If the player has also moved
	if (gridComp->GetFullyAligned().x && gridComp->GetFullyAligned().y)
	{
		// Update our current position in the graph and recalculate
		std::cout << "Update current position: " << nodeAI->GetPosition() << std::endl;

		for (const auto& player : levelManager.GetPlayers())
		{
			const auto gridPosPlayer = player->GetComponent<GridComponent>()->GetGridPos();
			GraphNode* nodePlayer    = g.GetNodeAtWorldPos(gridPosPlayer);

			auto path = astar.FindPath(nodeAI, nodePlayer);

			if (path.size() > 2)
			{
				currentEdge       = g.GetConnection(path[0]->GetIndex(), path[1]->GetIndex());
				useLocationCoords = false;
			}
			else
			{
				selectedPlayer    = player;
				useLocationCoords = true;
			}
		}
	}

	glm::vec2 from, to;
	// If we are closer then 1 grid cell to the actual player calculate on global coordinates
	// This is not a pretty solution. We actually want to bake floats into our grid but that for later
	if (useLocationCoords)
	{
		const auto pos       = GetEntity()->GetComponent<LocationComponent>()->GetTransform();
		const auto playerPos = selectedPlayer->GetComponent<LocationComponent>()->GetTransform();
		from                 = pos.GetPosition();
		to                   = playerPos.GetPosition();
	}
	else
	{
		const auto fromNode = g.GetNode(currentEdge->GetFrom());
		const auto toNode   = g.GetNode(currentEdge->GetTo());
		from                = fromNode->GetPosition();
		to                  = toNode->GetPosition();
	}
	if (to.x > from.x)
		moveRightCommand->Execute();
	else if (to.x < from.x)
		moveLeftCommand->Execute();
	else if (to.y > from.y)
		moveDownCommand->Execute();
	else if (to.y < from.y)
		moveUpCommand->Execute();
}

void dae::AIComponent::Render(float)
{
}
