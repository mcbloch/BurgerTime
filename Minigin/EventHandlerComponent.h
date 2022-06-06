#pragma once

#include "Component.h"
#include "Event.h"

namespace dae
{
	class EventHandlerComponent :
		public Component
	{
	public:
		EventHandlerComponent(
			std::shared_ptr<GameObject>);
		void Update(float) override;
		void Render(float) override;

		void AddEventListener(int    eventID,
		                      void (*callback)(std::shared_ptr<GameObject>, const Event*));
		void HandleEvent(const Event* pEvent) const;

	private:
		std::unordered_map<int, void (*)(std::shared_ptr<GameObject> go, const Event*)> m_CallbackMap{};
	};
}
