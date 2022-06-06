#include "MiniginPCH.h"

#include "GameObject.h"
#include "Component.h"

dae::GameObject::GameObject()
{
};

dae::GameObject::~GameObject()
{
	for (const auto comp : m_Components)
	{
		if (comp) { delete(comp); }
	}

	// This should delete the children as they are shared references.
	m_Children.clear();
};

void dae::GameObject::Update(float deltaTime)
{
	for (const auto& comp : m_Components)
	{
		comp->Update(deltaTime);
	}

	// Pass update to children
	for (const auto& child : m_Children)
	{
		child->Update(deltaTime);
	}

	// Cleanup components marked for deletion
	for (const auto& comp : m_Components)
	{
		if (comp->GetGravestone())
		{
			delete(comp);
		}
	}
	auto _it = std::ranges::remove_if(m_Components, [&](auto comp)
	{
		return comp->GetGravestone();
	});
}

void dae::GameObject::Render(float dt) const
{
	for (const auto comp : m_Components)
	{
		comp->Render(dt);
	}

	for (auto& child : m_Children)
	{
		child->Render(dt);
	}
}

void dae::GameObject::SetParent(GameObject* parent)
{
	m_Parent = parent;
}

dae::GameObject* dae::GameObject::GetParent() const
{
	return m_Parent;
}

size_t dae::GameObject::GetChildCount() const
{
	return m_Children.size();
}

std::shared_ptr<dae::GameObject> dae::GameObject::GetChildAt(int index) const
{
	return m_Children.at(index);
}

void dae::GameObject::RemoveChild(int index)
{
	auto child = m_Children.begin() + index;

	m_Children.erase(m_Children.begin() + index);
}

void dae::GameObject::AddChild(std::shared_ptr<GameObject> go)
{
	go->SetParent(this);
	m_Children.push_back(go);
}


void dae::GameObject::AddComponent(Component* c)
{
	m_Components.push_back(c);
	c->Init();
}
