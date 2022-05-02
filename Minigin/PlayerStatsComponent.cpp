#include "MiniginPCH.h"
#include "PlayerStatsComponent.h"

dae::PlayerStatsComponent::PlayerStatsComponent(const std::shared_ptr<GameObject> go, const int initialLives)
	: Component(go), m_InitialLives(initialLives), m_Lives(initialLives)
{
}

void dae::PlayerStatsComponent::ReduceRemainingLives()
{
	m_Lives -= 1;
}

void dae::PlayerStatsComponent::ChangePoints(const int amount)
{
	m_Points += amount;
}
