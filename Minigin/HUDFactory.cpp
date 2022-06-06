#include "MiniginPCH.h"
#include "HUDFactory.h"

#include "EventHandlerComponent.h"
#include "EventQueue.h"
#include "GameObject.h"
#include "GraphicsComponent.h"
#include "LocationComponent.h"
#include "PlayerComponent.h"
#include "ResourceManager.h"

std::shared_ptr<dae::GameObject> dae::HUDFactory::CreateGameObjectHUD(Scene& scene)
{
	const auto go = std::make_shared<GameObject>();
	go->AddComponent(new LocationComponent(go, {800, 60}));
	go->AddComponent(new TextComponent(
		go, "Lives: ",
		ResourceManager::GetInstance().LoadFont("Lingua.otf", 24)));
	scene.Add(go);

	const auto points = std::make_shared<GameObject>();
	points->AddComponent(new LocationComponent(points, {800, 80}));
	points->AddComponent(new TextComponent(points, "Points: ",
	                                       ResourceManager::GetInstance().LoadFont("Lingua.otf", 24)));
	const auto eventHandler = new EventHandlerComponent(points);
	eventHandler->AddEventListener(BurgerDrop, &OnBurgerDrop);
	points->AddComponent(eventHandler);
	go->AddChild(points);

	const auto lives = std::make_shared<GameObject>();
	lives->AddComponent(new LocationComponent(lives, {800, 40}));
	// TODO Add a better way to set the initial text
	lives->AddComponent(new TextComponent(lives, "Lives: 3",
	                                      ResourceManager::GetInstance().LoadFont("Lingua.otf", 24)));
	const auto eventHandlerPoints = new EventHandlerComponent(lives);
	eventHandlerPoints->AddEventListener(PlayerDie, &OnPlayerDie);
	lives->AddComponent(eventHandlerPoints);
	go->AddChild(lives);

	return go;
}


void dae::HUDFactory::OnPlayerDie(std::shared_ptr<GameObject> originalObject, const Event* pEvent)
{
	std::cout << "Ohno, he dieded" << std::endl;
	const auto& statsComp    = pEvent->GetOrigin()->GetComponent<PlayerComponent>();
	const auto  livesLeft    = statsComp->GetRemainingLives();
	const auto  initialLives = statsComp->GetInitialLives();

	originalObject->GetComponent<TextComponent>()->SetText(
		"Lives: " + std::to_string(livesLeft) + "/" + std::to_string(initialLives));
}

void dae::HUDFactory::OnBurgerDrop(std::shared_ptr<GameObject> originalObject, const Event* pEvent)
{
	const auto& statsComp = pEvent->GetOrigin()->GetComponent<PlayerComponent>();
	const auto  points    = statsComp->GetPoints() + 50;
	statsComp->ChangePoints(points);

	originalObject->GetComponent<TextComponent>()->SetText(
		"Points: " + std::to_string(points)
	);
}
