#pragma once
#include "Singleton.h"

namespace dae
{
	class LevelManager final : public dae::Singleton<LevelManager>
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

		[[nodiscard]] bool HasWalkablePiece(std::pair<int, int> gridPos) const;
		[[nodiscard]] bool HasLadderPiece(std::pair<int, int> gridPos) const;

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

		std::map<std::pair<int, int>, LevelObject> levelObjects{};

	private:
		friend class Singleton<LevelManager>;
	};
}
