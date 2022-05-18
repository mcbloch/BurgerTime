#include "MiniginPCH.h"
#include "LevelManager.h"

#include <fstream>
#include <sstream>
#include <string>

#include "GameObject.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SceneManager.h"

int dae::LevelManager::LoadNextLevel()
{
	// TODO destroy all old objects
	// TODO validate if there are more levels, maybe just wrap?
	currentLevel += 1;
	LoadLevel(currentLevel);
	return currentLevel;
}

int dae::LevelManager::LoadPreviousLevel()
{
	// TODO destroy all old objects
	// TODO validate if there are more levels, maybe just wrap?
	currentLevel -= 1;
	LoadLevel(currentLevel);
	return currentLevel;
}

void dae::LevelManager::CreateLevelObject(const LevelObject levelObject, const int x, const int y) const
{
	const auto scene = SceneManager::GetInstance().GetCurrentScene();

	std::string text;
	switch (levelObject)
	{
	case LevelObject::platform:
		text = "platform";
		break;
	case LevelObject::burger:
		text = "burger";
		break;
	case LevelObject::ladder:
		text = "ladder";
		break;
	default: ;
	}

	const auto go = std::make_shared<GameObject>();
	go->AddComponent(new TextComponent(
		go,
		text,
		ResourceManager::GetInstance().LoadFont("Lingua.otf", 10)));
	go->AddComponent(new LocationComponent(go, float(x), float(y)));
	scene->Add(go);
}

void dae::LevelManager::LoadLevel(int) const
{
	const auto scene = SceneManager::GetInstance().GetCurrentScene();
	const auto go    = std::make_shared<GameObject>();
	go->AddComponent(new SpriteMapTextureComponent(
		go, m_LevelSpriteFile,
		((currentLevel % 3) * (208 + 8)),
		((currentLevel / 3) * ((200 + 8) * 2)),
		208, 200, 2.5f));
	go->AddComponent(new LocationComponent(go, float(20), float(100)));
	scene->Add(go);

	return;
	//
	// std::ifstream infile(m_LevelDataPath + levelFile);
	// if (infile.is_open())
	// {
	// 	std::string line;
	// 	int         row = 0;
	// 	while (std::getline(infile, line))
	// 	{
	// 		// std::istringstream iss(line);
	// 		// int a, b;
	// 		// if (!(iss >> a >> b)) { break; } // error
	//
	// 		// process pair (a,b)
	//
	// 		for (int col = 0; col < line.size(); col++)
	// 		{
	// 			const int x = 10 + (col * gridSize);
	// 			const int y = 10 + (row * gridSize);
	// 			switch (line[col])
	// 			{
	// 			case '-': CreateLevelObject(LevelObject::platform, x, y);
	// 			case '=': CreateLevelObject(LevelObject::burger, x, y);
	// 			case '|': CreateLevelObject(LevelObject::ladder, x, y);
	// 			default: ;
	// 			}
	// 		}
	//
	// 		row += 1;
	// 	}
	// }
	// else
	// {
	// 	throw std::runtime_error("Could not open level file: " + levelFile);
	// }
}
