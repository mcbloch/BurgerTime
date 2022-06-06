#include "MiniginPCH.h"
#include "MoveComponent.h"

#include "GameObject.h"
#include "GridComponent.h"
#include "LevelManager.h"
#include "LocationComponent.h"

dae::MoveComponent::MoveComponent(std::shared_ptr<GameObject> go, int speed): Component(go), m_Speed(speed)
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
	glm::vec2 dirVector;

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
	default: break;
	}

	const auto location = GetEntity()->GetComponent<LocationComponent>();
	const auto grid     = GetEntity()->GetComponent<GridComponent>();
	const auto pos      = location->GetPosition();

	// Some rounding so we keep it even :)
	// We should actually just use int vectors but that's a refactor for later.
	const auto newPosInt = glm::ivec2(pos + (dirVector * float(m_Speed)));
	const auto newPos    = glm::vec2(newPosInt);

	// std::cout << "New pos: " << newPosInt << std::endl;


	// Check if movement is allowed. We return early in these conditionals if the move input is invalid
	if (direction == Direction::Up || direction == Direction::Down)
	{
		// We can not move onto a ladder when we are not fully aligned to a vertical grid square
		if (!grid->GetFullyAligned().x) return;

		if (grid->GetFullyAligned().y)
		{
			int checkOffsetY = 0;
			if (direction == Direction::Down)
				checkOffsetY = 1;

			if (!LevelManager::GetInstance().HasLadderPiece(grid->GetGridPos() + glm::ivec2{ 0, checkOffsetY }))
				return;
			else
				std::cout << std::endl;
		}
	}
	else
	{
		// We can not move while hanging in the air or midway a ladder
		if (!grid->GetFullyAligned().y) return;

		// We don't need to check for a floor is we are not aligned as we are already on 2 pieces then.
		// IMPORTANT: This assumes that our movement never skips over an alignment fase.
		if (grid->GetFullyAligned().x)
		{
			const bool floorFound = LevelManager::GetInstance().HasWalkablePiece(
				grid->GetGridPos() + glm::ivec2{dirVector.x, 0}
			);
			if (!floorFound) return;
		}
	}

	location->SetPosition(newPos);
	grid->UpdateGridPos();
}
