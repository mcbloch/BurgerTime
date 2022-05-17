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
		void CreateLevelObject(const LevelObject levelObject, int x, int y) const;
		void LoadLevel(int level) const;

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

	private:
		friend class Singleton<LevelManager>;
	};
}
