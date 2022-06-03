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

	// std::cout << "Loading level object '" << text << "' at location " << col << ", " << row << std::endl;

	// const auto go = std::make_shared<GameObject>();
	// go->AddComponent(new TextComponent(
	// 	go,
	// 	text,
	// 	ResourceManager::GetInstance().LoadFont("Lingua.otf", 10)));
	// go->AddComponent(new LocationComponent(go, float(x), float(y)));
	// scene->Add(go);

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
				case '_': obj = LevelObject::platform;
					break;
				case '=': obj = LevelObject::burger;
					break;
				case '|': obj = LevelObject::ladder;
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
		auto& obj = levelObjects.at({x, y});
		return obj == LevelObject::platform || obj == LevelObject::ladder;
	}
	return false;
}

bool dae::LevelManager::HasLadderPiece(std::pair<int, int> gridPos) const
{
	auto [x, y] = gridPos;
	if (levelObjects.contains({x, y}))
	{
		return levelObjects.at({x, y}) == LevelObject::ladder;
	}
	return false;
}
