#pragma once
#include "EventHandler.h"
#include "GameObject.h"

namespace dae
{
	class HUD :
		public std::enable_shared_from_this<HUD>,
		public EventHandler
	{
	public:
		HUD();
		void InitHandlers();

		void                        HandleEvent(const Event* pEvent) override;
		std::shared_ptr<GameObject> GetGameObject() { return m_GameObject; };
	private:
		std::shared_ptr<GameObject> m_PointsObject;

		std::shared_ptr<GameObject> m_GameObject;

		static void UpdatePoints(std::shared_ptr<GameObject> originalObject, const Event* pEvent);
	};
}
