#include "MiniginPCH.h"
#include "HUD.h"

#include "EventHandlerComponent.h"
#include "EventQueue.h"
#include "GameObject.h"
#include "PlayerStatsComponent.h"
#include "ResourceManager.h"

dae::HUD::HUD(): m_GameObject(std::make_shared<GameObject>())
{
	m_GameObject->AddComponent(new LocationComponent(m_GameObject, 800, 60));
	m_GameObject->AddComponent(new TextComponent(
		m_GameObject, "Lives: ",
		ResourceManager::GetInstance().LoadFont("Lingua.otf", 24)));


	const auto points = std::make_shared<GameObject>();
	points->AddComponent(new LocationComponent(points, 800, 80));
	points->AddComponent(new TextComponent(points, "Points: ",
	                                       ResourceManager::GetInstance().LoadFont("Lingua.otf", 24)));
	const auto eventHandlerComp = std::make_shared<EventHandlerComponent>(points, BurgerDrop, &UpdatePoints);
	points->AddComponent(eventHandlerComp.get());
	m_GameObject->AddChild(points);
}

void dae::HUD::InitHandlers()
{
	// Example on how to listen for events
	const auto self    = shared_from_this();
	auto       handler = std::static_pointer_cast<EventHandler>(self);
	EventQueue::GetInstance().AttachEvent(Die, handler);
	EventQueue::GetInstance().AttachEvent(BurgerDrop, handler);
	EventQueue::GetInstance().AttachEvent(EnemyDie, handler);
}

void dae::HUD::HandleEvent(const Event* pEvent)
{
	// Example on how to handle an event
	auto self = shared_from_this();
	switch (pEvent->GetID())
	{
	case Die:
		{
			std::cout << "Ohno, he dieded" << std::endl;
			const auto statsComp    = pEvent->GetOrigin()->GetComponent<PlayerStatsComponent>();
			const auto livesLeft    = statsComp->GetRemainingLives();
			const auto initialLives = statsComp->GetInitialLives();
			m_GameObject->GetComponent<TextComponent>()->SetText(
				"Lives: " + std::to_string(livesLeft) + "/" + std::to_string(initialLives));
			break;
		}
	default: break;
	}
}


void dae::HUD::UpdatePoints(std::shared_ptr<GameObject> originalObject, const Event* pEvent)
{
	const auto statsComp = pEvent->GetOrigin()->GetComponent<PlayerStatsComponent>();
	const auto points    = statsComp->GetPoints();

	originalObject->GetComponent<TextComponent>()->SetText(
		"Points: " + std::to_string(points)
	);
}
