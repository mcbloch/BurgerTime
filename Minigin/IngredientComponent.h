#pragma once
#include "Component.h"

namespace dae
{
	class IngredientComponent : public Component
	{

		void Update(float) override;
		void Render(float dt) override;
	};
}
