#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"

using namespace dae;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name)
{
}

Scene::~Scene() = default;

void Scene::Add(const std::shared_ptr<GameObject>& object)
{
	m_Objects.push_back(object);
}

void Scene::Update(float deltaTime)
{
	for (auto& object : m_Objects)
	{
		object->Update(deltaTime);
	}

	std::erase_if(m_Objects, [&](std::shared_ptr<GameObject> obj)
	{
		return obj->GetGravestone();
	});
}

void Scene::Render(float dt) const
{
	for (const auto& object : m_Objects)
	{
		object->Render(dt);
	}
}
