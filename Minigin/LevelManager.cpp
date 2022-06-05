#include "MiniginPCH.h"
#include "LevelManager.h"

#include <fstream>
#include <sstream>
#include <string>

#include "GameObject.h"
#include "GridComponent.h"
#include "GridGraph.h"
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

void dae::LevelManager::RegisterPlayer(const std::shared_ptr<GameObject> p)
{
	m_PlayerObjects.push_back(p);
}

void dae::LevelManager::ClearPlayers()
{
	m_PlayerObjects.clear();
}

dae::GameObjects& dae::LevelManager::GetPlayers()
{
	return m_PlayerObjects;
}

void dae::LevelManager::CreateLevelObject(const LevelObject levelObject, const int col, const int row)
{
	const auto scene = SceneManager::GetInstance().GetCurrentScene();

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

	m_LevelObjects.emplace(std::make_pair(col, row), levelObject);
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

	int maxRow = 0;
	int maxCol = 0;

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

				if (col > maxCol) maxCol = col;
			}

			if (row > maxRow) maxRow = row;
			row += 1;
		}
	}
	else
	{
		throw std::runtime_error("Could not open level file: " + levels[levelIndex]);
	}

	auto directionOffsets = std::vector<std::pair<float, float>>{{0.f, -1.f}, {-1.f, 0.f}};

	for (int col = 0; col <= maxCol; col++)
	{
		for (int row = 0; row <= maxRow; row++)
		{
			if (!m_LevelObjects.contains({col, row})) continue;

			const auto currNode = new GraphNode(m_LevelGraph.GetNextFreeNodeIndex(), Vector2(float(col), float(row)));
			m_LevelGraph.AddNode(currNode);
			std::cout << "Node: " << (*currNode).GetPosition() << std::endl;

			for (const auto [offsetX, offsetY] : directionOffsets)
			{
				const float newCol = float(col) + offsetX;
				const float newRow = float(row) + offsetY;
				if (newCol < 0.f || newRow < 0.f)
					continue;

				if (m_LevelObjects.contains({int(newCol), int(newRow)}))
				{
					const auto otherNodeIdx = m_LevelGraph.GetNodeIdxAtWorldPos(
						Vector2(newCol, newRow));
					if (otherNodeIdx != invalid_node_index)
					{
						const auto newConn = new GraphConnection(currNode->GetIndex(), otherNodeIdx);
						// std::cout << "Conn: " << (*newConn).GetFrom() << "-" << (*newConn).GetTo() << std::endl;
						m_LevelGraph.AddConnection(newConn);
					}
					else
					{
						throw(new std::runtime_error("Could not retrieve node from graph but it should be there..."));
					}
				}
			}
		}
	}
}

bool dae::LevelManager::HasWalkablePiece(std::pair<int, int> gridPos) const
{
	auto [x, y] = gridPos;
	if (m_LevelObjects.contains({x, y}))
	{
		return true;
		// auto& obj = m_LevelObjects.at({x, y});
		// return obj == LevelObject::Platform || obj == LevelObject::Ladder;
	}
	return false;
}

bool dae::LevelManager::HasLadderPiece(std::pair<int, int> gridPos) const
{
	auto [x, y] = gridPos;
	if (m_LevelObjects.contains({x, y}))
	{
		return m_LevelObjects.at({x, y}) == LevelObject::Ladder;
	}
	return false;
}

dae::Graph<dae::GraphNode, dae::GraphConnection>& dae::LevelManager::GetLevelGraph()
{
	return m_LevelGraph;
}
