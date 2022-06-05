#pragma once
#include "Command.h"
#include "Component.h"

namespace dae
{
	class AIComponent :
		public Component
	{
	public:
		explicit AIComponent(std::shared_ptr<GameObject>);

		void Update(float) override;
		void Render(float) override;

		std::unique_ptr<MoveLeftCommand> moveLeftCommand = std::make_unique<MoveLeftCommand>(GetEntity());
		std::unique_ptr<MoveRightCommand> moveRightCommand = std::make_unique<MoveRightCommand>(GetEntity());
		std::unique_ptr<MoveUpCommand> moveUpCommand = std::make_unique<MoveUpCommand>(GetEntity());
		std::unique_ptr<MoveDownCommand> moveDownCommand = std::make_unique<MoveDownCommand>(GetEntity());
	};
}
