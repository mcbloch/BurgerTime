#pragma once
#include "Event.h"
#include "EventHandler.h"
#include "GameObject.h"
#include "ResourceManager.h"


namespace dae
{
	class PeterPepperGameObject :
		public std::enable_shared_from_this<PeterPepperGameObject>,
		public EventHandler
	{
	public:
		PeterPepperGameObject();
		void InitHandlers();

		void Die();


		void                        HandleEvent(const Event* pEvent) override;
		std::shared_ptr<GameObject> GetGameObject() { return m_GameObject; };
	private:
		std::shared_ptr<GameObject> m_GameObject;
	};
}
