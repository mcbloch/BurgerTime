#pragma once
#include "Component.h"

namespace dae
{
	/**
	 * Gets the player and AI objects from the level
	 */
	class IngredientComponent : public Component
	{
	public:
		explicit IngredientComponent(const std::shared_ptr<GameObject>& gameObject)
			: Component(gameObject)
		{
		}

		inline static glm::ivec2 IngredientLocationOffset = {8, 28};
	private:
		void Update(float) override;
		void Render(float dt) override;

		// void RegisterPlayer();
		// void RegisterAI();

		bool       inMotion = false;
		glm::bvec3 marked{};
	};
}
