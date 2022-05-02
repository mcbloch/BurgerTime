#include "MiniginPCH.h"
#include "PeterPepperGameObject.h"

#include "EventHandler.h"
#include "EventQueue.h"
#include "InputManager.h"
#include "PlayerStatsComponent.h"


dae::PeterPepperGameObject::PeterPepperGameObject() : m_GameObject(std::make_shared<GameObject>())
{
	m_GameObject->AddComponent(new LocationComponent(m_GameObject, 80, 40));
	m_GameObject->AddComponent(new PlayerStatsComponent(m_GameObject, 5));
}

void dae::PeterPepperGameObject::InitHandlers()
{
	// Example on how to listen for events
	const auto self    = shared_from_this();
	auto       handler = std::static_pointer_cast<EventHandler>(self);
	EventQueue::GetInstance().AttachEvent(Events::Die, handler);
}

void dae::PeterPepperGameObject::Die()
{
	// Example on how to send events
	m_GameObject->GetComponent<PlayerStatsComponent>()->ReduceRemainingLives();
	EventQueue::GetInstance().SendEvent(Events::Die, m_GameObject);
}

void dae::PeterPepperGameObject::HandleEvent([[maybe_unused]] const Event* pEvent)
{
	// Example on how to handle an event
	// auto self = shared_from_this();
	// switch (pEvent->GetID())
	// {
	// default: break;
	// }
}
