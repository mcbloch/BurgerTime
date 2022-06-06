#pragma once

#include "Component.h"
#include "GameObject.h"

namespace dae
{
	class LocationComponent :
		public Component
	{
	public:
		LocationComponent() = default;
		LocationComponent(std::shared_ptr<GameObject> go, glm::vec2 pos);

		void Update(float) override;
		void Render(float) override;

		[[nodiscard]] glm::vec2 GetPosition() const
		{
			if (GetEntity()->GetParent())
			{
				return m_Position + GetEntity()->GetParent()->GetComponent<LocationComponent>()->GetPosition();
			}
			return m_Position;
		}

		void SetPosition(glm::vec2 pos);

		~LocationComponent() override                                = default;
		LocationComponent(const LocationComponent& other)            = delete;
		LocationComponent(LocationComponent&& other)                 = delete;
		LocationComponent& operator=(const LocationComponent& other) = delete;
		LocationComponent& operator=(LocationComponent&& other)      = delete;

	private:
		glm::vec2 m_Position;
	};
}
