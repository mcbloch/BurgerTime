#pragma once

#include "Component.h"
#include "Transform.h"

namespace dae
{
	class LocationComponent :
		public Component
	{
	public:
		LocationComponent() = default;
		LocationComponent(std::shared_ptr<GameObject> go, float x, float y);

		void Update(float) override;
		void Render(float) override;

		[[nodiscard]] Transform GetTransform() const { return m_Transform; }
		void                    SetPosition(float x, float y);

		~LocationComponent() override                                = default;
		LocationComponent(const LocationComponent& other)            = delete;
		LocationComponent(LocationComponent&& other)                 = delete;
		LocationComponent& operator=(const LocationComponent& other) = delete;
		LocationComponent& operator=(LocationComponent&& other)      = delete;

	private:
		Transform m_Transform;
	};
}