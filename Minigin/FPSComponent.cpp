#include "MiniginPCH.h"
#include "FPSComponent.h"

#include <functional>

dae::FPSComponent::FPSComponent(std::shared_ptr<GameObject> go) :
	Component(go)
{
}

void dae::FPSComponent::Update(float dt)
{
	const float fps = 1.f / dt;

	std::stringstream ss;
	ss << std::fixed << std::setprecision(1) << fps;
	const std::string fps_s = ss.str();

	*m_Text = fps_s;
}

void dae::FPSComponent::Render(float)
{
}

std::shared_ptr<std::string> dae::FPSComponent::GetTextLink()
{
	return m_Text;
}
