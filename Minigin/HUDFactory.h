#pragma once

#include "EventHandlerComponent.h"
#include "GameObject.h"
#include "Scene.h"

namespace dae
{
	class HUDFactory
	{
	public:
		static std::shared_ptr<GameObject> CreateGameObjectHUD(Scene& scene);

	private:
		HUDFactory() = default;

		static void OnBurgerDrop(std::shared_ptr<GameObject> originalObject, const Event* pEvent);
		static void OnPlayerDie(std::shared_ptr<GameObject> originalObject, const Event* pEvent);
	};
}
