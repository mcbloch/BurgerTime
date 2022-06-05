#include "MiniginPCH.h"
#include "LocationComponent.h"

dae::LocationComponent::LocationComponent(
	const std::shared_ptr<GameObject> go,
	const glm::vec2 pos) : Component(go)
{
	SetPosition(pos);
}

void dae::LocationComponent::Update(float)
{
}

void dae::LocationComponent::Render(float)
{
	return void();
}

void dae::LocationComponent::SetPosition(const glm::vec2 pos)
{
	m_Transform.SetPosition(pos);
}
