#pragma once
#include "Component.h"

namespace dae
{
	class GameObject;

	class FPSComponent : public Component
	{
	public:
		FPSComponent(std::shared_ptr<GameObject> go);

		void Update(float dt) override;
		void Render(float dt) override;

		std::shared_ptr<std::string> GetTextLink();

	private:
		std::shared_ptr<std::string> m_Text = std::make_shared<std::string>();
	};
}
