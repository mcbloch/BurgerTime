#include "MiniginPCH.h"
#include "PlayerComponent.h"

dae::PlayerComponent::PlayerComponent(const std::shared_ptr<GameObject> go, const int initialLives)
	: Component(go), m_InitialLives(initialLives), m_Lives(initialLives)
{
}

void dae::PlayerComponent::ReduceRemainingLives()
{
	m_Lives -= 1;
}

void dae::PlayerComponent::ChangePoints(const int amount)
{
	m_Points += amount;
}
