#include "MiniginPCH.h"
#include "Component.h"

dae::Component::Component(const std::shared_ptr<GameObject> obj): m_Entity(obj)
{
}

void dae::Component::SetEntity(const std::shared_ptr<GameObject> gameObject)
{
	m_Entity = gameObject;
}
