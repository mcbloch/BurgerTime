#pragma once
#include "Component.h"

namespace dae
{
	/**
	 * Keeps a reference to the player object
	 */
	class IngredientComponent : public Component
	{
		void Update(float) override;
		void Render(float dt) override;

		void RegisterPlayer();
		void RegisterAI();
	};
}
