#include "MiniginPCH.h"
#include "IngredientComponent.h"

#include "EventQueue.h"
#include "GameObject.h"
#include "GridComponent.h"
#include "LevelManager.h"
#include "LocationComponent.h"

void dae::IngredientComponent::Update(float)
{
	auto&      levelManager       = LevelManager::GetInstance();
	const auto locationIngredient = GetEntity()->GetComponent<LocationComponent>();
	const auto posIngredientBase  = locationIngredient->GetPosition();

	if (!inMotion)
	{
		for (const auto& player : levelManager.GetPlayers())
		{
			const auto locationPlayer = player->GetComponent<LocationComponent>();
			const auto posPlayer      = locationPlayer->GetPosition();
			const int  playerCenterX  = int(posPlayer.x + (GridComponent::GridCellSize.x / 2));

			// std::cout << "Player: " << posPlayer << std::endl;
			for (int i = 0; i < 3; i++)
			{
				if (!marked[i])
				{
					const auto posIngredient =
						glm::ivec2(posIngredientBase) + (i * glm::ivec2{GridComponent::GridCellSize.x, 0}) - glm::ivec2{
							0, IngredientLocationOffset.y
						};
					// std::cout << "Burger : " << posIngredient << std::endl;
					if (posIngredient.y == (int(posPlayer.y)) &&
						playerCenterX > posIngredient.x && playerCenterX < (posIngredient.x +
							GridComponent::GridCellSize.
							x))
					{
						marked[i] = true;
						std::cout << "Burger ingredient hit!" << std::endl;
					}
				}
			}

			if (all(marked))
			{
				std::cout << "Burger ingredient complete. Dropping..." << std::endl;
				EventQueue::GetInstance().SendEvent(BurgerDrop, player);

				marked   = {false, false, false};
				inMotion = true;
			}
		}
	}
	else
	{
		// Move directly via the location component. Stop when aligned on a floor piece
		locationIngredient->SetPosition(posIngredientBase + glm::vec2{0, 2});
	}
}

void dae::IngredientComponent::Render(float)
{
}
