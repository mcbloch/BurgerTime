#include "MiniginPCH.h"
#include "Highscores.h"

#include <fstream>
#include <stdio.h>

#include "GameObject.h"
#include "GraphicsComponent.h"
#include "LocationComponent.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "curl/curl.h"

using string = std::string;
using json = nlohmann::json;

//
//  libcurl write callback function
//

static int writer(char*        data, size_t size, size_t nmemb,
                  std::string* writerData)
{
	if (writerData == nullptr)
		return 0;

	writerData->append(data, size * nmemb);

	return int(size * nmemb);
}

namespace ns
{
} // namespace ns

dae::Highscores::Highscores()
{
	const auto& scene = SceneManager::GetInstance().GetCurrentScene();
	highscoreObject   = std::make_shared<GameObject>();
	highscoreObject->AddComponent(new LocationComponent(highscoreObject, {800, 150}));
	highscoreObject->AddComponent(new TextComponent(
		highscoreObject,
		"Highscores \n────────── \nLoading... ",
		ResourceManager::GetInstance().LoadFont("Nouveau_IBM_Stretch.ttf", 24)));
	scene.get()->Add(highscoreObject);
}

std::vector<dae::Highscore> dae::Highscores::FetchHighscores()
{
	curl_global_init(CURL_GLOBAL_DEFAULT);

	CURL*    conn;
	CURLcode code;

	// libcurl variables for error strings and returned data
	static char        errorBuffer[CURL_ERROR_SIZE];
	static std::string buffer;

	highscoreObject->GetComponent<TextComponent>()->SetText(
		string{"Highscores \n────────── \nFailed to load"});

	conn = curl_easy_init();
	if (conn == nullptr)
	{
		fprintf(stderr, "Failed to create CURL connection\n");
		return {};
	}

	code = curl_easy_setopt(conn, CURLOPT_ERRORBUFFER, errorBuffer);
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to set error buffer [%d]\n", code);
		return {};
	}

	code = curl_easy_setopt(conn, CURLOPT_URL, std::string{serverUrl + "/highscores"}.c_str());
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to set URL [%s]\n", errorBuffer);
		return {};
	}

	code = curl_easy_setopt(conn, CURLOPT_FOLLOWLOCATION, 1L);
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to set redirect option [%s]\n", errorBuffer);
		return {};
	}

	code = curl_easy_setopt(conn, CURLOPT_WRITEFUNCTION, writer);
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to set writer [%s]\n", errorBuffer);
		return {};
	}

	code = curl_easy_setopt(conn, CURLOPT_WRITEDATA, &buffer);
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to set write data [%s]\n", errorBuffer);
		return {};
	}

	code = curl_easy_perform(conn);
	curl_easy_cleanup(conn);

	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to get highscores [%s]\n", errorBuffer);
		return {};
	}

	std::vector<Highscore> highscores{};
	std::string            highText{};
	for (auto jhigh : json::parse(buffer))
	{
		auto high = jhigh.get<Highscore>();
		highscores.push_back(high);

		highText += high.name;
		highText += ": ";
		highText += std::to_string(high.score);
		highText += " \n";
	}

	highscoreObject->GetComponent<TextComponent>()->SetText(
		string{"Highscores \n────────── \n"} + highText);

	return highscores;
}
