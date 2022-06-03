#include "MiniginPCH.h"
#include "MoveComponent.h"

#include "GameObject.h"
#include "GridComponent.h"
#include "LevelManager.h"
#include "LocationComponent.h"

dae::MoveComponent::MoveComponent(std::shared_ptr<GameObject> go): Component(go)
{
}

void dae::MoveComponent::Update(float)
{
}

void dae::MoveComponent::Render(float)
{
}

void dae::MoveComponent::Move(const Direction direction) const
{
	std::pair<int, int> dirVector;

	switch (direction)
	{
	case Direction::Up: dirVector = {0, -1};
		break;
	case Direction::Left: dirVector = {-1, 0};
		break;
	case Direction::Right: dirVector = {1, 0};
		break;
	case Direction::Down: dirVector = {0, 1};
		break;
	default: ;
	}

	const auto location = GetEntity()->GetComponent<LocationComponent>();
	const auto grid     = GetEntity()->GetComponent<GridComponent>();
	const auto pos      = location->GetTransform().GetPosition();

	const auto newX = pos.x + (dirVector.first * m_Speed);
	const auto newY = pos.y + (dirVector.second * m_Speed);

	// Check if movement is allowed. We return early in these conditionals if the move input is invalid
	if (direction == Direction::Up || direction == Direction::Down)
	{
		// We can not move onto a ladder when we are not fully aligned to a vertical grid square
		if (!grid->GetXFullyAligned()) return;

		if (grid->GetYFullyAligned())
		{
			auto gridPos      = grid->GetGridPos();
			int  checkOffsetY = 0;
			if (direction == Direction::Down)
				checkOffsetY = -1;

			if (!LevelManager::GetInstance().HasLadderPiece({gridPos.first, gridPos.second - checkOffsetY}))
				return;
		}
	}
	else
	{
		// We can not move while hanging in the air or midway a ladder
		if (!grid->GetYFullyAligned()) return;

		// We don't need to check for a floor is we are not aligned as we are already on 2 pieces then.
		// IMPORTANT: This assumes that our movement never skips over an alignment fase.
		if (grid->GetXFullyAligned())
		{
			auto       gridPos    = grid->GetGridPos();
			const bool floorFound = LevelManager::GetInstance().HasWalkablePiece({
				gridPos.first + dirVector.first, gridPos.second
			});
			if (!floorFound) return;
		}
	}

	location->SetPosition(newX, newY);
}
