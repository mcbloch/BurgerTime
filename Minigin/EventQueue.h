#pragma once
#include <functional>

#include "Singleton.h"

#include <queue>

#include "Event.h"
#include "EventHandler.h"

namespace dae
{
	class EventQueue final : public Singleton<EventQueue>
	{
	public:
		void SendEvent(int eventId, std::shared_ptr<GameObject> go);
		void AttachEvent(int eventId, std::shared_ptr<EventHandler> handler);
		void DetachEvent(int eventId, std::shared_ptr<EventHandler> handler);

		void ProcessQueue();
	private:
		friend class Singleton<EventQueue>;
		EventQueue() = default;

		std::queue<Event*>                                                  queue{};
		std::unordered_map<int, std::vector<std::shared_ptr<EventHandler>>> handlers{};
	};
}
