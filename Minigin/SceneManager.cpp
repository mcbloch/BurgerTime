#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update(float deltaTime)
{
	for (const auto& scene : m_Scenes)
	{
		scene->Update(deltaTime);
	}
}

void dae::SceneManager::Render(float dt)
{
	for (const auto& scene : m_Scenes)
	{
		scene->Render(dt);
	}
}

std::shared_ptr<dae::Scene> dae::SceneManager::GetCurrentScene() const
{
	return m_Scenes.back();
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_Scenes.push_back(scene);
	return *scene;
}
