#include "MiniginPCH.h"
#include "Minigin.h"

// #include <steam_api_common.h>
#include <thread>

#include "AudioEventQueue.h"
#include "EventQueue.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "GraphicsComponent.h"
#include "HUD.h"
#include "Locator.h"
#include "LoggedAudio.h"
#include "PeterPepperGameObject.h"
#include "Scene.h"
#include "SDLMixAudio.h"

using namespace std;

void PrintSDLVersion()
{
	SDL_version compiled{};
	SDL_version linked{};

	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);
	printf("We compiled against SDL version %d.%d.%d ...\n",
	       compiled.major, compiled.minor, compiled.patch);
	printf("We are linking against SDL version %d.%d.%d.\n",
	       linked.major, linked.minor, linked.patch);
}

void dae::Minigin::Initialize()
{
	PrintSDLVersion();

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		1040, // 640
		680,  // 480
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_Window);
}

/**
 * Code constructing the scene world starts here
 */
void dae::Minigin::LoadGame()
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	//auto go = std::make_shared<GameObject>();
	//go->AddComponent(new TextureComponent("background.jpg"));
	//scene.Add(go);

	//go = std::make_shared<GameObject>();
	//go->AddComponent(new TextureComponent("logo.png"));
	//go->SetPosition(216, 180);
	//scene.Add(go);

	peterPepper = std::make_shared<PeterPepperGameObject>();
	scene.Add(peterPepper->GetGameObject());
	peterPepper->InitHandlers();

	const auto hud = std::make_shared<HUD>();
	hud->InitHandlers();
	scene.Add(hud->GetGameObject());

	auto go = std::make_shared<GameObject>();
	go->AddComponent(new TextComponent(
		go, "Programming 4 Assignment",
		ResourceManager::GetInstance().LoadFont("Lingua.otf", 36)));
	go->AddComponent(new LocationComponent(go, 80, 40));
	scene.Add(go);

	// Example that children work
	const auto fpsHolder = std::make_shared<GameObject>();
	scene.Add(fpsHolder);

	go = std::make_shared<GameObject>();
	go->AddComponent(new FPSComponent(go));
	go->AddComponent(new LocationComponent(go, 0, 0));
	fpsHolder->AddChild(go);

	go = std::make_shared<GameObject>();
	go->AddComponent(new FPSPhysicsComponent(go));
	go->AddComponent(new LocationComponent(go, 0, 20));
	fpsHolder->AddChild(go);

	peterPepper->Die();
}

void dae::Minigin::Cleanup()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");

	// Setup the locators
	Locator::Provide(new SDLMixAudio());
	enableAudioLogging();

	AudioEventQueue::GetInstance().SendEvent(PlaySound, { PINK_PANTHER });

	LoadGame();

	{
		auto& renderer     = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();
		auto& input        = InputManager::GetInstance();
		auto& eventQueue   = EventQueue::GetInstance();

		bool doContinue = true;

		float t  = 0.f;
		float dt = MsPerFrame / 1000.f;

		auto  currentTime = std::chrono::high_resolution_clock::now();
		float accumulator = 0.f;

		while (doContinue)
		{
			auto  newTime   = std::chrono::high_resolution_clock::now();
			float frameTime = std::chrono::duration<float>(newTime - currentTime).count();
			currentTime     = newTime;

			accumulator += frameTime;

			// Add a catchup in fixed steps
			// lag += deltaTime;
			doContinue = input.ProcessInput();
			eventQueue.ProcessQueue();
			/*SteamAPI_RunCallbacks();*/

			while (accumulator >= dt)
			{
				// Do a fixed update of max x ms per frame.
				sceneManager.Update(dt);
				accumulator -= dt;
				t += dt;
			}

			renderer.Render(frameTime);

			// VSync
			// const auto sleepTime = currentTime + std::chrono::milliseconds(MsPerFrame) - std::chrono::high_resolution_clock::now();
			// this_thread::sleep_for(sleepTime);
		}
	}

	Cleanup();
}

void dae::Minigin::enableAudioLogging()
{
	// Decorate the existing service.
	Audio* service = new LoggedAudio(Locator::GetAudio());

	// Swap it in.
	Locator::Provide(service);
}