#pragma once
#include "GameObject.h"
#include "GridComponent.h"
#include "MoveComponent.h"
#include "AIComponent.h"
#include "Scene.h"
#include "SceneManager.h"

namespace dae
{
	class AIFactory
	{
	public:
		static void CreateGameObjectMrHotDog(Scene& scene)
		{
			auto go = std::make_shared<GameObject>();
			go->AddComponent(new LocationComponent(go, 260, 450));
			go->AddComponent(new GridComponent(go));
			go->AddComponent(new MoveComponent(go));
			// go->AddComponent(new AIComponent(go));

			go->AddComponent(new SpriteMapTextureComponent(
				go, "sprites/Arcade - Burger Time - Characters & Objects - Opaque.png",
				0, 2 * 16, 16, 16, 2.5f));

			scene.Add(go);
		}
	private:
		// Static class that can not be instantiated
		AIFactory(){}
	};
}
