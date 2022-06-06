#include "MiniginPCH.h"
#include "PeterPepperFactory.h"

#include "EventHandler.h"
#include "EventHandlerComponent.h"
#include "EventQueue.h"
#include "GraphicsComponent.h"
#include "GridComponent.h"
#include "InputManager.h"
#include "LocationComponent.h"
#include "MoveComponent.h"
#include "PlayerComponent.h"

std::shared_ptr<dae::GameObject> dae::PeterPepperFactory::CreateGameObjectPeterPepper(Scene& scene)
{
	const auto go = std::make_shared<GameObject>();
	go->AddComponent(new LocationComponent(go, {200, 450}));
	go->AddComponent(new GridComponent(go));
	go->AddComponent(new MoveComponent(go, 2));
	go->AddComponent(new PlayerComponent(go, 3));
	go->AddComponent(new SpriteMapTextureComponent(
		go, "sprites/Arcade - Burger Time - Characters & Objects - Opaque.png",
		0, 0, 16, 16, 2.5f));
	// go->AddComponent(new EventHandlerComponent(go, ));

	const auto& input = InputManager::GetInstance();

	input.BindButtonCommand({ControllerButton::DPadLeft, ButtonState::Down},
	                        std::make_unique<MoveLeftCommand>(go));
	input.BindButtonCommand({ControllerButton::DPadRight, ButtonState::Down},
	                        std::make_unique<MoveRightCommand>(go));
	input.BindButtonCommand({ControllerButton::DPadUp, ButtonState::Down},
	                        std::make_unique<MoveUpCommand>(go));
	input.BindButtonCommand({ControllerButton::DPadDown, ButtonState::Down},
	                        std::make_unique<MoveDownCommand>(go));

	input.BindKeyCommand({SDL_SCANCODE_LEFT, ButtonState::Pressed}, std::make_unique<MoveLeftCommand>(go));
	input.BindKeyCommand({SDL_SCANCODE_RIGHT, ButtonState::Pressed}, std::make_unique<MoveRightCommand>(go));
	input.BindKeyCommand({SDL_SCANCODE_UP, ButtonState::Pressed}, std::make_unique<MoveUpCommand>(go));
	input.BindKeyCommand({SDL_SCANCODE_DOWN, ButtonState::Pressed}, std::make_unique<MoveDownCommand>(go));

	scene.Add(go);
	return go;
}

void dae::PeterPepperFactory::InitHandlers()
{
	// Example on how to listen for events
	// const auto self    = shared_from_this();
	// auto       handler = std::static_pointer_cast<EventHandler>(self);
	// EventQueue::GetInstance().AttachEvent(Events::Die, handler);
}

void dae::PeterPepperFactory::Die()
{
	// Example on how to send events
	// m_GameObject->GetComponent<PlayerStatsComponent>()->ReduceRemainingLives();
	// EventQueue::GetInstance().SendEvent(Events::Die, m_GameObject);
}

void dae::PeterPepperFactory::HandleEvent([[maybe_unused]] const Event* pEvent)
{
	// Example on how to handle an event
	// auto self = shared_from_this();
	// switch (pEvent->GetID())
	// {
	// default: break;
	// }
}
