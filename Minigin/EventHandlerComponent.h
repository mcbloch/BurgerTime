#pragma once
#include <functional>

#include "Component.h"
#include "Event.h"
#include "EventHandler.h"

namespace dae
{
	class EventHandlerComponent :
		public Component
	{
	public:
		EventHandlerComponent(
			std::shared_ptr<GameObject>,
			int                        eventID,
			void (*                    callback)(std::shared_ptr<GameObject>, const Event*));
		void Init() override;
		void Update(float) override;
		void Render(float) override;

		void HandleEvent(const Event* pEvent) const;

	private:
		int    m_EventID;
		void (*m_Callback)(std::shared_ptr<GameObject> go, const Event*);
	};
}
