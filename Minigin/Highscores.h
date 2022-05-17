#pragma once
#include "GameObject.h"

namespace dae
{
	struct Highscore
	{
		std::string name{};
		int         score{};
	};

	inline void to_json(nlohmann::json& j, const Highscore& p)
	{
		j = nlohmann::json{{"name", p.name}, {"score", p.score}};
	}

	inline void from_json(const nlohmann::json& j, Highscore& p)
	{
		j.at("name").get_to(p.name);
		j.at("score").get_to(p.score);
	}

	class Highscores
	{
	public:
		Highscores();
		std::vector<Highscore> FetchHighscores();

	private:
		std::string serverUrl{"http://localhost:8000"};
		std::shared_ptr<GameObject> highscoreObject;
	};
}
