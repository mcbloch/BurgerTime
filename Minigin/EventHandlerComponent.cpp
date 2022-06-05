#include "MiniginPCH.h"
#include "EventHandlerComponent.h"

#include "EventQueue.h"

dae::EventHandlerComponent::EventHandlerComponent(
	std::shared_ptr<GameObject> go, const int eventID,
	void (*                     callback)(std::shared_ptr<GameObject>, const Event*))
	: Component(go), m_EventID(eventID), m_Callback(callback)
{
}

void dae::EventHandlerComponent::Init()
{
	// Example on how to listen for events
	EventQueue::GetInstance().AttachEvent(m_EventID, GetEntity());
}

void dae::EventHandlerComponent::Update(float)
{
}

void dae::EventHandlerComponent::Render(float)
{
}


void dae::EventHandlerComponent::HandleEvent([[maybe_unused]] const Event* pEvent) const
{
	if (pEvent->GetID() == m_EventID)
	{
		m_Callback(GetEntity(), pEvent);
	}
}
