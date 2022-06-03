#pragma once
#include "Singleton.h"

namespace dae
{
	class LevelManager final : public dae::Singleton<LevelManager>
	{
		enum class LevelObject
		{
			platform,
			burger,
			ladder,
		};

	public:
		int LoadNextLevel();
		int LoadPreviousLevel();

	private:
		void CreateLevelObject(LevelObject levelObject, int x, int y);
		void LoadLevel(int level);

		bool HasFloorPiece(int x, int y) const;
		bool HasLadderPiece(int x, int y) const;

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
		int         gridSize          = 50;

		std::map<std::pair<int, int>, LevelObject> levelObjects{};

	private:
		friend class Singleton<LevelManager>;
	};
}
