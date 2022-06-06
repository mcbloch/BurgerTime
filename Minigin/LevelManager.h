#pragma once
#include "GameObject.h"
#include "Graph.h"
#include "GraphConnection.h"
#include "GraphNode.h"
#include "Singleton.h"

namespace dae
{
	using GameObjects = std::vector<std::shared_ptr<GameObject>>;

	class LevelManager final : public Singleton<LevelManager>
	{
		enum class LevelObject
		{
			Platform,
			Ladder,
			BurgerTop,
			BurgerLettuce,
			BurgerMeat,
			BurgerBottom
		};

	public:
		int LoadNextLevel();
		int LoadPreviousLevel();

		std::shared_ptr<GameObject> GetLevelGameObject();

		void         RegisterPlayer(std::shared_ptr<GameObject> p);
		void         ClearPlayers();
		GameObjects& GetPlayers();

		void         RegisterEnemy(std::shared_ptr<GameObject> e);
		void         ClearEnemies();
		GameObjects& GetEnemies();

		[[nodiscard]] bool HasWalkablePiece(glm::ivec2 gridPos) const;
		[[nodiscard]] bool HasLadderPiece(glm::ivec2 gridPos) const;

		Graph<GraphNode, GraphConnection>& GetLevelGraph();
		void                               ResetLevel() const;

	private:
		void CreateLevelObject(LevelObject levelObject, int col, int row);
		void LoadLevel(int levelIndex);

		int                        currentLevel = -1;
		std::array<std::string, 6> levels{
			"level_1.txt",
			"level_1.txt",
			"level_1.txt",
			"level_1.txt",
			"level_1.txt",
			"level_1.txt"
		};
		std::string m_LevelDataPath   = "../Data/levels/";
		std::string m_LevelSpriteFile = "sprites/Arcade - Burger Time - Stages.png";

		// Owned by the LevelManager
		std::shared_ptr<GameObject>                 m_LevelGameObject;
		std::unordered_map<glm::ivec2, LevelObject> m_LevelObjects{};
		Graph<GraphNode, GraphConnection>           m_LevelGraph{false};

		// Not owned
		GameObjects m_PlayerObjects{};
		GameObjects m_EnemyObjects{};

	private:
		friend class Singleton<LevelManager>;
	};
}
