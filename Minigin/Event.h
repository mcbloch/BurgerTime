#pragma once
#include "GameObject.h"

namespace dae
{
	enum Events
	{
		PlayerDie,
		BurgerDrop,
		EnemyDie
	};

	class Event
	{
	public:
		Event(int id, std::shared_ptr<GameObject> origin);

		[[nodiscard]] int                         GetID() const;
		[[nodiscard]] std::shared_ptr<GameObject> GetOrigin() const;;

	private:
		int                         m_Id;
		std::shared_ptr<GameObject> m_Origin{};
	};
}
