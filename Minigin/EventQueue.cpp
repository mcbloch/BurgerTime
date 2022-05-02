#include "MiniginPCH.h"
#include "EventQueue.h"

void dae::EventQueue::SendEvent(const int eventId, const std::shared_ptr<GameObject> go)
{
	// Events are owned by the event queue
	queue.push(new Event(eventId, go));
}

void dae::EventQueue::AttachEvent(const int eventId, const std::shared_ptr<EventHandler> handler)
{
	if (!handlers.contains(eventId))
	{
		handlers.insert({eventId, {}});
	}

	handlers.at(eventId).push_back(handler);
}

void dae::EventQueue::DetachEvent(const int eventId, const std::shared_ptr<EventHandler> handler)
{
	if (handlers.contains(eventId))
	{
		auto _it = std::ranges::remove(handlers.at(eventId), handler);
	}
}

void dae::EventQueue::ProcessQueue()
{
	if (!queue.empty())
	{
		const Event* event = queue.front();
		queue.pop();

		for (const auto& handler : handlers.at(event->GetID()))
		{
			handler->HandleEvent(event);
		}

		delete(event);
	}
}
