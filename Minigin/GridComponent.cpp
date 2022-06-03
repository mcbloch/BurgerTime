#include "MiniginPCH.h"
#include "GridComponent.h"

#include "GameObject.h"

dae::GridComponent::GridComponent(
	const std::shared_ptr<GameObject> go) :
	Component(go)
{
}

void dae::GridComponent::Update(float)
{
	auto& pos = GetEntity()->GetComponent<LocationComponent>()->GetTransform().GetPosition();

	const int basePosX = (int(pos.x) - GridBaseX);
	const int basePosY = (int(pos.y) - GridBaseY);

	const int newGridCol = basePosX / GridCellSizeX;
	const int newGridRow = basePosY / GridCellSizeY;

	isXFullyAligned = (basePosX % GridCellSizeX) == 0;
	isYFullyAligned = (basePosY % GridCellSizeY) == 0;

	if (newGridCol != m_GridC)
	{
		printf("Changed column: %d, %d -> %d, %d\n", m_GridC, m_GridR, newGridCol, newGridRow);
		// TODO Launch event of changing column
	}
	if (newGridRow != m_GridR)
	{
		printf("Changed row: %d, %d -> %d, %d\n", m_GridC, m_GridR, newGridCol, newGridRow);
		// TODO Launch event of changing row
	}

	m_GridC = newGridCol;
	m_GridR = newGridRow;
}

void dae::GridComponent::Render(float)
{
}

std::pair<int, int> dae::GridComponent::GetGridPos()
{
	return std::make_pair(m_GridC, m_GridR);
}

bool dae::GridComponent::GetXFullyAligned() const
{
	return isXFullyAligned;
}

bool dae::GridComponent::GetYFullyAligned() const
{
	return isYFullyAligned;
}