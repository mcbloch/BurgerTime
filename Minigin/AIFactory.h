#pragma once
#include "GameObject.h"
#include "SceneManager.h"

namespace dae
{
	class AIFactory
	{
	public:
		static std::shared_ptr<GameObject> CreateGameObjectMrHotDog(Scene& scene, glm::ivec2 startPos);

	private:
		// Static class that can not be instantiated
		AIFactory() = default;
	};
}
