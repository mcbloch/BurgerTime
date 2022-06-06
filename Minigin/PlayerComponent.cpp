#include "MiniginPCH.h"
#include "PlayerComponent.h"

#include "GridComponent.h"
#include "LevelManager.h"
#include "LocationComponent.h"

dae::PlayerComponent::PlayerComponent(const std::shared_ptr<GameObject> go, const int initialLives)
	: Component(go), m_InitialLives(initialLives), m_Lives(initialLives)
{
}

void dae::PlayerComponent::Update(float)
{
	const auto location = GetEntity()->GetComponent<LocationComponent>();
	auto       pos      = location->GetPosition();
	for (const auto& enemy : LevelManager::GetInstance().GetEnemies())
	{
		// Detect if we collide with an enemy
		auto enemyPos = enemy->GetComponent<LocationComponent>()->GetPosition();

		// We will asume the width of peter and the pepper to be half a cell
		const float w = float(GridComponent::GridCellSize.x);
		const float h = float(GridComponent::GridCellSize.y);
		if (pos.x < enemyPos.x + w &&
			pos.x + w > enemyPos.x &&
			pos.y < enemyPos.y + h &&
			h + pos.y > enemyPos.y)
		{
			// collision detected!
			// Take life away if we die.
			ReduceRemainingLives();
			EventQueue::GetInstance().SendEvent(PlayerDie, GetEntity());
			LevelManager::GetInstance().ResetLevel();
		}
	}
}

void dae::PlayerComponent::ReduceRemainingLives()
{
	m_Lives -= 1;
}

void dae::PlayerComponent::ChangePoints(const int amount)
{
	m_Points += amount;
}
