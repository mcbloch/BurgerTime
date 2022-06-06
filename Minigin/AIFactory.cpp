#include "MiniginPCH.h"
#include "AIFactory.h"

#include "Scene.h"
#include "AIComponent.h"
#include "GraphicsComponent.h"
#include "GridComponent.h"
#include "LocationComponent.h"
#include "MoveComponent.h"

std::shared_ptr<dae::GameObject> dae::AIFactory::CreateGameObjectMrHotDog(Scene& scene, glm::ivec2 startPos = {0, 0})
{
	auto go = std::make_shared<GameObject>();
	go->AddComponent(new LocationComponent(go, {})); // overwritten by GridComponent
	go->AddComponent(new GridComponent(go, startPos));
	go->AddComponent(new MoveComponent(go, 1));
	go->AddComponent(new AIComponent(go));

	go->AddComponent(new SpriteMapTextureComponent(
		go, "sprites/Arcade - Burger Time - Characters & Objects - Opaque.png",
		0, 2 * 16, 16, 16, 2.5f));

	scene.Add(go);
	return go;
}
