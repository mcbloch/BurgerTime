#pragma once
#include <functional>

#include "Component.h"
#include "Event.h"
#include "EventHandler.h"

namespace dae
{
	class EventHandlerComponent :
		public Component,
		public std::enable_shared_from_this<EventHandlerComponent>,
		public EventHandler
	{
	public:
		EventHandlerComponent(std::shared_ptr<GameObject>                                       ,
		                      int                                                               eventID,
		                      std::function<void(std::shared_ptr<GameObject> go, const Event*)> callback);
		void Init() override;
		void Update(float) override;
		void Render(float) override;

		void HandleEvent(const Event* pEvent) override;

	private:
		int                                                            m_EventID;
		std::function<void(std::shared_ptr<GameObject>, const Event*)> m_Callback;
	};
}
