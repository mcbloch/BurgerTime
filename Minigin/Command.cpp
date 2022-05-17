#include "MiniginPCH.h"
#include "Command.h"

#include <iostream>

#include "AudioEventQueue.h"
#include "LevelManager.h"

Command::Command(std::shared_ptr<dae::GameObject> actor)
	: mActor(actor)
{
}

void MusicPauseCommand::Execute()
{
	dae::AudioEventQueue::GetInstance().SendEvent(dae::PauseSounds, {});
}


constexpr int speed = 10;

void MoveUpCommand::Execute()
{
	const auto locComp = mActor->GetComponent<dae::LocationComponent>();
	const auto pos = locComp->GetTransform().GetPosition();
	locComp->SetPosition(pos.x, pos.y - speed);
}

void MoveRightCommand::Execute()
{
	const auto locComp = mActor->GetComponent<dae::LocationComponent>();
	const auto pos = locComp->GetTransform().GetPosition();
	locComp->SetPosition(pos.x + speed, pos.y);
}

void MoveLeftCommand::Execute()
{
	const auto locComp = mActor->GetComponent<dae::LocationComponent>();
	const auto pos = locComp->GetTransform().GetPosition();
	locComp->SetPosition(pos.x - speed, pos.y);
}

void MoveDownCommand::Execute()
{
	const auto locComp = mActor->GetComponent<dae::LocationComponent>();
	const auto pos = locComp->GetTransform().GetPosition();
	locComp->SetPosition(pos.x, pos.y + speed);
}

void NextLevelCommand::Execute()
{
	dae::LevelManager::GetInstance().LoadNextLevel();
}

void PreviousLevelCommand::Execute()
{
	dae::LevelManager::GetInstance().LoadPreviousLevel();
}

// void BrakeCommand::Execute(const short value)
// {
// if (value != lastValue)
// {
// std::cout << "Trying to brake " << static_cast<int>((value / 255.f) * 100.f) << "%" << std::endl;
// lastValue = value;
// }
// }
