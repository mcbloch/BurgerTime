#pragma once
#include "Event.h"
#include "EventHandler.h"
#include "GameObject.h"
#include "Scene.h"


namespace dae
{
	class PeterPepperFactory :
		public EventHandler
	{
	public:
		static std::shared_ptr<GameObject> CreateGameObjectPeterPepper(std::shared_ptr<GameObject>& level);
		static void                        InitHandlers();

		void Die();

		void HandleEvent(const Event* pEvent) override;

	private:
		// Static class that can not be instantiated
		PeterPepperFactory() = default;
	};
}
