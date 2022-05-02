#pragma once

#include "Component.h"
#include "LocationComponent.h"
#include "GraphicsComponent.h"

namespace dae
{
	class GameObject final
	{
	public:
		void Update(float deltaTime);
		void Render(float dt) const;

		/* Manage Components */
		void AddComponent(Component* c);

		template <typename T>
		T* GetComponent();

		template <typename T>
		void RemoveComponent() const;


		/* Manage relation between game objects */
		void                      SetParent(GameObject* parent);
		[[nodiscard]] GameObject* GetParent() const;

		[[nodiscard]] size_t                      GetChildCount() const;
		[[nodiscard]] std::shared_ptr<GameObject> GetChildAt(int index) const;
		void                                      RemoveChild(int index);
		void                                      AddChild(std::shared_ptr<GameObject> go);


		GameObject();
		~GameObject();
		GameObject(const GameObject& other)            = delete;
		GameObject(GameObject&& other)                 = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other)      = delete;

	private:
		GameObject*                              m_Parent     = nullptr;
		std::vector<std::shared_ptr<GameObject>> m_Children   = {};
		std::vector<Component*>                  m_Components = {};
	};

	template <typename T>
	T* GameObject::GetComponent()
	{
		for (auto comp : m_Components)
		{
			if (dynamic_cast<T*>(comp) != nullptr)
			{
				return dynamic_cast<T*>(comp);
			}
		}
		return nullptr;
	}

	template <typename T>
	void GameObject::RemoveComponent() const
	{
		for (auto comp : m_Components)
		{
			if (dynamic_cast<T*>(comp) != nullptr)
			{
				comp->MarkForRemoval();
			}
		}
	}
}
