#include "MiniginPCH.h"
#include "LocationComponent.h"

dae::LocationComponent::LocationComponent(
	const std::shared_ptr<GameObject> go,
	const float                       x, const float y) : Component(go)
{
	SetPosition(x, y);
}

void dae::LocationComponent::Update(float)
{
}

void dae::LocationComponent::Render(float)
{
	return void();
}

void dae::LocationComponent::SetPosition(const float x, const float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}
