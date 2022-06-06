#include "MiniginPCH.h"
#include "GridComponent.h"
#include "LocationComponent.h"

#include "GameObject.h"

dae::GridComponent::GridComponent(
	const std::shared_ptr<GameObject> go) :
	Component(go)
{
	const auto location = go->GetComponent<LocationComponent>();
	m_assert(location != nullptr,
	         "The GridComponent needs a LocationComponent to work. Verify that you added one before the GridComponent");

	m_GridPosStart = GetGridPosFromLocation(location->GetPosition());
}

dae::GridComponent::GridComponent(std::shared_ptr<GameObject> go, const glm::ivec2 gridPosStart)
	: Component(go), m_GridPosStart(gridPosStart)
{
	const auto location = go->GetComponent<LocationComponent>();
	m_assert(location != nullptr,
	         "The GridComponent needs a LocationComponent to work. Verify that you added one before the GridComponent");

	// Set the initial location based on the grid
	location->SetPosition(GetLocationFromGridPos(gridPosStart));
}


void dae::GridComponent::ResetToStart() const
{
	const auto location = GetEntity()->GetComponent<LocationComponent>();
	location->SetPosition(GetLocationFromGridPos(m_GridPosStart));
}

void dae::GridComponent::Update(float)
{
	UpdateGridPos();
}

void dae::GridComponent::Render(float)
{
}


glm::ivec2 dae::GridComponent::GetGridPos() const
{
	return m_GridPos;
}

void dae::GridComponent::UpdateGridPos()
{
	const auto pos = GetEntity()->GetComponent<LocationComponent>()->GetPosition();

	m_GridPos = GetGridPosFromLocation(pos);

	const auto posInt = glm::ivec2(pos);
	isFullyAligned.x  = ((posInt.x - GridBase.x) % GridCellSize.x) == 0;
	isFullyAligned.y  = ((posInt.y - GridBase.y) % GridCellSize.y) == 0;
}

glm::bvec2 dae::GridComponent::GetFullyAligned() const
{
	return isFullyAligned;
}

glm::vec2 dae::GridComponent::GetLocationFromGridPos(const glm::ivec2 gridPos)
{
	return (gridPos * GridCellSize);
}

glm::ivec2 dae::GridComponent::GetGridPosFromLocation(const glm::vec2 pos)
{
	return (glm::ivec2(pos)) / GridCellSize;
}
