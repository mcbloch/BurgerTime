#include "MiniginPCH.h"
#include "EventHandlerComponent.h"

#include "EventQueue.h"

dae::EventHandlerComponent::EventHandlerComponent(
	const std::shared_ptr<GameObject>                                    go,
	const int                                                            eventID,
	const std::function<void(std::shared_ptr<GameObject>, const Event*)> callback)
	: Component(go), m_EventID(eventID), m_Callback(callback)
{
}

void dae::EventHandlerComponent::Init()
{
	// Example on how to listen for events
	const auto self    = shared_from_this();
	auto       handler = std::static_pointer_cast<EventHandler>(self);
	EventQueue::GetInstance().AttachEvent(m_EventID, handler);
}

void dae::EventHandlerComponent::Update(float)
{
}

void dae::EventHandlerComponent::Render(float)
{
}


void dae::EventHandlerComponent::HandleEvent([[maybe_unused]] const Event* pEvent)
{
	if (pEvent->GetID() == m_EventID)
	{
		m_Callback(GetEntity(), pEvent);
	}
}
