#pragma once
#include "SceneManager.h"
#include "GameObject.h"
#include "LocationComponent.h"
#include "GraphicsComponent.h"

namespace dae
{
	class SceneObject;

	class Scene
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(const std::shared_ptr<GameObject>& object);

		void Update(float deltaTime);
		void Render(float dt) const;

		~Scene();
		Scene(const Scene& other)            = delete;
		Scene(Scene&& other)                 = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other)      = delete;

	private:
		explicit Scene(const std::string& name);

		std::string                              m_Name;
		std::vector<std::shared_ptr<GameObject>> m_Objects{};

		static unsigned int m_IdCounter;
	};
}
