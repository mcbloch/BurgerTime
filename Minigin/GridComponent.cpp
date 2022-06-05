#include "MiniginPCH.h"
#include "GridComponent.h"
#include "LocationComponent.h"

#include "GameObject.h"

dae::GridComponent::GridComponent(
	const std::shared_ptr<GameObject> go) :
	Component(go)
{
}

void dae::GridComponent::Update(float)
{
	auto& pos = GetEntity()->GetComponent<LocationComponent>()->GetTransform().GetPosition();

	const glm::ivec2 basePos    = glm::ivec2(pos) - GridBase;
	const glm::ivec2 newGridCol = basePos / GridCellSize;

	isFullyAligned.x = (basePos.x % GridCellSize.x) == 0;
	isFullyAligned.y = (basePos.y % GridCellSize.y) == 0;

	m_GridPos = newGridCol;
}

void dae::GridComponent::Render(float)
{
}


glm::ivec2 dae::GridComponent::GetGridPos() const
{
	return m_GridPos;
}

glm::bvec2 dae::GridComponent::GetFullyAligned() const
{
	return isFullyAligned;
}
