#include "MiniginPCH.h"
#include "EventHandlerComponent.h"

#include "EventQueue.h"

dae::EventHandlerComponent::EventHandlerComponent(
	std::shared_ptr<GameObject> go)
	: Component(go)
{
}

void dae::EventHandlerComponent::Update(float)
{
}

void dae::EventHandlerComponent::Render(float)
{
}

void dae::EventHandlerComponent::AddEventListener(int    eventID,
                                                  void (*callback)(std::shared_ptr<GameObject>, const Event*))
{
	m_CallbackMap.insert({eventID, callback});
	EventQueue::GetInstance().AttachEvent(eventID, GetEntity());
}


void dae::EventHandlerComponent::HandleEvent([[maybe_unused]] const Event* pEvent) const
{
	if (m_CallbackMap.contains(pEvent->GetID()))
	{
		m_CallbackMap.at(pEvent->GetID())(GetEntity(), pEvent);
	}
}
