#pragma once
#include "Component.h"
#include "EventQueue.h"

namespace dae
{
	class GameObject;

	class PlayerComponent : public Component
	{
	public:
		PlayerComponent() = default;
		PlayerComponent(std::shared_ptr<GameObject> go, int initialLives);

		[[nodiscard]] int GetInitialLives() const { return m_InitialLives; };
		[[nodiscard]] int GetRemainingLives() const { return m_Lives; }
		[[nodiscard]] int GetPoints() const { return m_Points; }

		void Update(float) override;

		void Render(float) override
		{
		}

		void ReduceRemainingLives();
		void ChangePoints(int amount);
	private:
		const int m_InitialLives;
		int       m_Lives;

		int m_Points = 0;
	};
}
