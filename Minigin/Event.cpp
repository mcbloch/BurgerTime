#include "MiniginPCH.h"
#include "Event.h"

dae::Event::Event(const int id, const std::shared_ptr<GameObject> origin) :
	m_Id(id), m_Origin(origin)
{
}

int dae::Event::GetID() const
{
	return m_Id;
}

std::shared_ptr<dae::GameObject> dae::Event::GetOrigin() const
{
	return m_Origin;
}
