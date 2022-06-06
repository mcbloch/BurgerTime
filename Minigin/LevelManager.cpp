#include "MiniginPCH.h"
#include "LevelManager.h"

#include <fstream>
#include <string>

#include "GameObject.h"
#include "GridComponent.h"
#include "IngredientComponent.h"
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

void dae::LevelManager::RegisterEnemy(std::shared_ptr<GameObject> e)
{
	m_EnemyObjects.push_back(e);
}

void dae::LevelManager::ClearEnemies()
{
	m_EnemyObjects.clear();
}

dae::GameObjects& dae::LevelManager::GetEnemies()
{
	return m_EnemyObjects;
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
			glm::vec2(GridComponent::GridBase) +
			glm::vec2{col, row} * glm::vec2(GridComponent::GridCellSize) +
			glm::vec2(IngredientComponent::IngredientLocationOffset)
		));
		go->AddComponent(new IngredientComponent(go));
		m_LevelGameObject->AddChild(go);
	}

	m_LevelObjects.insert(std::make_pair(glm::ivec2{col, row}, levelObject));
}

void dae::LevelManager::LoadLevel(const int levelIndex)
{
	const auto scene  = SceneManager::GetInstance().GetCurrentScene();
	m_LevelGameObject = std::make_shared<GameObject>();
	m_LevelGameObject->AddComponent(new SpriteMapTextureComponent(
		m_LevelGameObject, m_LevelSpriteFile,
		((currentLevel % 3) * (208 + 8)),
		((currentLevel / 3) * ((200 + 8) * 2)),
		208, 200, 2.5f));
	m_LevelGameObject->AddComponent(new LocationComponent(m_LevelGameObject, {20, 100}));
	scene->Add(m_LevelGameObject);

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

	const auto directionOffsets = std::vector<glm::ivec2>{{0, -1}, {-1, 0}};

	for (int col = 0; col <= maxCol; col++)
	{
		for (int row = 0; row <= maxRow; row++)
		{
			if (!m_LevelObjects.contains({col, row})) continue;

			const auto currNode = new GraphNode(m_LevelGraph.GetNextFreeNodeIndex(), glm::vec2{float(col), float(row)});
			m_LevelGraph.AddNode(currNode);
			std::cout << "Node: " << (*currNode).GetPosition() << std::endl;

			for (const auto offset : directionOffsets)
			{
				const glm::ivec2 newPos = glm::ivec2{col, row} + offset;
				if (newPos.x < 0 || newPos.y < 0)
					continue;

				if (m_LevelObjects.contains(newPos))
				{
					const auto otherNodeIdx = m_LevelGraph.GetNodeIdxAtWorldPos(newPos);
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

bool dae::LevelManager::HasWalkablePiece(const glm::ivec2 gridPos) const
{
	if (m_LevelObjects.contains(gridPos))
	{
		// All pieces specified in our map file are walkable at the moment
		return true;

		// auto& obj = m_LevelObjects.at({x, y});
		// return obj == LevelObject::Platform || obj == LevelObject::Ladder;
	}
	return false;
}

bool dae::LevelManager::HasLadderPiece(const glm::ivec2 gridPos) const
{
	if (m_LevelObjects.contains(gridPos))
	{
		return m_LevelObjects.at(gridPos) == LevelObject::Ladder;
	}
	return false;
}

dae::Graph<dae::GraphNode, dae::GraphConnection>& dae::LevelManager::GetLevelGraph()
{
	return m_LevelGraph;
}

void dae::LevelManager::ResetLevel() const
{
	// TODO These should be done as eventhandlers in the objects themselves
	for (const auto& p : m_PlayerObjects)
	{
		p->GetComponent<GridComponent>()->ResetToStart();
	}
	for (const auto& e : m_EnemyObjects)
	{
		e->GetComponent<GridComponent>()->ResetToStart();
	}
	// m_LevelObjects.clear();
	// m_LevelGraph.Clear();
	// m_LevelGameObject->MarkForRemoval();
	// for (const auto& enemy : m_EnemyObjects)
	// {
	// 	enemy->MarkForRemoval();
	// }
	// m_EnemyObjects.clear();
	// LoadLevel(currentLevel);
}
