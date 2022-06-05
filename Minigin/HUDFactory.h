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

		static void UpdateLives(std::shared_ptr<GameObject> originalObject, const Event* pEvent);
		static void UpdatePoints(std::shared_ptr<GameObject> originalObject, const Event* pEvent);
	};
}
