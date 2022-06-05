#include "MiniginPCH.h"
#include "AIComponent.h"

#include "BFS.h"
#include "Command.h"
#include "GameObject.h"
#include "GridComponent.h"
#include "LevelManager.h"

dae::AIComponent::AIComponent(std::shared_ptr<GameObject> go) : Component(go)
{
}

void dae::AIComponent::Update(float)
{
	auto& levelManager = LevelManager::GetInstance();
	// TODO Add some caching into the path-finding. This does not need to be done every frame.
	auto& g = levelManager.GetLevelGraph();

	auto bfs = BFS(&g);
	// Get my node in the graph
	const auto gridPos       = GetEntity()->GetComponent<GridComponent>()->GetGridPos();
	const auto gridPosV      = Vector2(float(gridPos.first), float(gridPos.second));
	const auto graphNodeFrom = g.GetNodeAtWorldPos(gridPosV);

	for (const auto& player : levelManager.GetPlayers())
	{
		const auto gridPosPlayer  = player->GetComponent<GridComponent>()->GetGridPos();
		const auto gridPosPlayerV = Vector2(float(gridPosPlayer.first), float(gridPosPlayer.second));
		const auto graphNodeTo    = g.GetNodeAtWorldPos(gridPosPlayerV);

		auto path = bfs.FindPath(graphNodeFrom, graphNodeTo);

		if (path.size() > 0)
		{
			const auto               nextNodeToMoveTo = path[0];
			if (nextNodeToMoveTo->GetPosition().x > gridPos.first)
				moveRightCommand->Execute();
			else if (nextNodeToMoveTo->GetPosition().x < gridPos.first)
				moveLeftCommand->Execute();
			else if (nextNodeToMoveTo->GetPosition().y > gridPos.second)
				moveDownCommand->Execute();
			else if (nextNodeToMoveTo->GetPosition().y > gridPos.second)
				moveUpCommand->Execute();
		}
		else
		{
			// We are in the same place as the player.
			// Player should be dead and we stop moving.
		}
	}
}

void dae::AIComponent::Render(float)
{
}
