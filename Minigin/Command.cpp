#include "MiniginPCH.h"
#include "Command.h"

#include <iostream>

void MoveUpCommand::Execute()
{
	std::cout << "Move up" << std::endl;
}

void MoveRightCommand::Execute()
{
	std::cout << "Move right" << std::endl;
}

void MoveLeftCommand::Execute()
{
	std::cout << "Move left" << std::endl;
}

void MoveDownCommand::Execute()
{
	std::cout << "Move down" << std::endl;
}

// void BrakeCommand::Execute(const short value)
// {
// if (value != lastValue)
// {
// std::cout << "Trying to brake " << static_cast<int>((value / 255.f) * 100.f) << "%" << std::endl;
// lastValue = value;
// }
// }
