#include "MiniginPCH.h"
#include "LevelManager.h"

#include <fstream>
#include <sstream>
#include <string>

#include "GameObject.h"
#include "GridComponent.h"
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

void dae::LevelManager::CreateLevelObject(const LevelObject levelObject, const int col, const int row)
{
	const auto scene = SceneManager::GetInstance().GetCurrentScene();

	// std::string text;
	// switch (levelObject)
	// {
	// case LevelObject::Platform:
	// 	text = "platform";
	// 	break;
	// case LevelObject::Ladder:
	// 	text = "ladder";
	// 	break;
	// default: ;
	// }

	// std::cout << "Loading level object '" << text << "' at location " << col << ", " << row << std::endl;

	// const auto go = std::make_shared<GameObject>();
	// go->AddComponent(new TextComponent(
	// 	go,
	// 	text,
	// 	ResourceManager::GetInstance().LoadFont("Lingua.otf", 10)));
	// go->AddComponent(new LocationComponent(go, float(x), float(y)));
	// scene->Add(go);

	int burgerYIndex = -1;
	switch (levelObject)
	{
	case LevelObject::BurgerTop:
		burgerYIndex = 0;
		break;
	case LevelObject::BurgerLettuce:
		burgerYIndex = 5;
		break;
	case LevelObject::BurgerMeat:
		burgerYIndex = 3;
		break;
	case LevelObject::BurgerBottom:
		burgerYIndex = 1;
		break;
	default: break;
	}

	if (burgerYIndex >= 0)
	{
		const auto go = std::make_shared<GameObject>();
		go->AddComponent(new SpriteMapTextureComponent(
			go, "sprites/Arcade - Burger Time - Characters & Objects - Opaque.png",
			7 * 16, 48 + (burgerYIndex * 8), 32, 8, 2.5f));
		go->AddComponent(new LocationComponent(
			go,
			float(GridComponent::GridBaseX + col * GridComponent::GridCellSizeX + 8),
			float(GridComponent::GridBaseY + row * GridComponent::GridCellSizeY + 28)
		));
		scene->Add(go);
	}

	levelObjects.emplace(std::make_pair(col, row), levelObject);
}

void dae::LevelManager::LoadLevel(const int levelIndex)
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

	std::ifstream infile(m_LevelDataPath + levels[levelIndex]);
	if (infile.is_open())
	{
		std::string line;
		int         row = 0;
		while (std::getline(infile, line))
		{
			// std::istringstream iss(line);
			// int a, b;
			// if (!(iss >> a >> b)) { break; } // error

			// process pair (a,b)

			for (int col = 0; col < line.size(); col++)
			{
				LevelObject obj;
				switch (line[col])
				{
				case '_': obj = LevelObject::Platform;
					break;
				case '|': obj = LevelObject::Ladder;
					break;
				case 'T': obj = LevelObject::BurgerTop;
					break;
				case 'L': obj = LevelObject::BurgerLettuce;
					break;
				case 'M': obj = LevelObject::BurgerMeat;
					break;
				case 'B': obj = LevelObject::BurgerBottom;
					break;
				case ' ': continue;
				default:
					throw std::runtime_error(
						std::string("Got an unexpected error while parsing the level file: ") + line[col]);
				}

				CreateLevelObject(obj, col, row);
			}

			row += 1;
		}
	}
	else
	{
		throw std::runtime_error("Could not open level file: " + levels[levelIndex]);
	}
}

bool dae::LevelManager::HasWalkablePiece(std::pair<int, int> gridPos) const
{
	auto [x, y] = gridPos;
	if (levelObjects.contains({x, y}))
	{
		return true;
		// auto& obj = levelObjects.at({x, y});
		// return obj == LevelObject::Platform || obj == LevelObject::Ladder;
	}
	return false;
}

bool dae::LevelManager::HasLadderPiece(std::pair<int, int> gridPos) const
{
	auto [x, y] = gridPos;
	if (levelObjects.contains({x, y}))
	{
		return levelObjects.at({x, y}) == LevelObject::Ladder;
	}
	return false;
}
