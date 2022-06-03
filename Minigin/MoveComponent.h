#pragma once
#include "Component.h"

namespace dae
{
	enum class Direction
	{
		Up,
		Left,
		Right,
		Down
	};

	class MoveComponent : public Component
	{
	public:
		MoveComponent(std::shared_ptr<GameObject> go);

		void Update(float) override;
		void Render(float) override;

		void Move(Direction) const;


		~MoveComponent() override                            = default;
		MoveComponent(const MoveComponent& other)            = delete;
		MoveComponent(MoveComponent&& other)                 = delete;
		MoveComponent& operator=(const MoveComponent& other) = delete;
		MoveComponent& operator=(MoveComponent&& other)      = delete;

	private:
		const int m_Speed = 10;
	};
}
