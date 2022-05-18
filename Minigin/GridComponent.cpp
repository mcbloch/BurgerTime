#include "MiniginPCH.h"
#include "GridComponent.h"

#include "GameObject.h"

dae::GridComponent::GridComponent(
	const std::shared_ptr<GameObject> go,
	const int                         gridBaseX,
	const int                         gridBaseY,
	const int                         gridCellSize) :
	Component(go), m_GridBaseX(gridBaseX), m_GridBaseY(gridBaseY), m_GridCellSize(gridCellSize)
{
}

void dae::GridComponent::Update(float)
{
	auto& pos = GetEntity()->GetComponent<LocationComponent>()->GetTransform().GetPosition();

	const int newGridCol = (int(pos.x) - m_GridBaseX) / m_GridCellSize;
	const int newGridRow = (int(pos.y) - m_GridBaseY) / m_GridCellSize;

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
