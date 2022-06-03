#include "MiniginPCH.h"
#include "Command.h"

#include "AudioEventQueue.h"
#include "LevelManager.h"
#include "MoveComponent.h"

Command::Command(std::shared_ptr<dae::GameObject> actor)
	: mActor(actor)
{
}

void MusicPauseCommand::Execute()
{
	dae::AudioEventQueue::GetInstance().SendEvent(dae::PauseSounds, {});
}


void MoveUpCommand::Execute()
{
	mActor->GetComponent<dae::MoveComponent>()->Move(dae::Direction::Up);
}

void MoveRightCommand::Execute()
{
	mActor->GetComponent<dae::MoveComponent>()->Move(dae::Direction::Right);
}

void MoveLeftCommand::Execute()
{
	mActor->GetComponent<dae::MoveComponent>()->Move(dae::Direction::Left);
}

void MoveDownCommand::Execute()
{
	mActor->GetComponent<dae::MoveComponent>()->Move(dae::Direction::Down);
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
