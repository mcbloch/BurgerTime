#pragma once
#include <thread>

#include "Locator.h"
#include "PeterPepperGameObject.h"
#include "SDLMixAudio.h"
struct SDL_Window;

namespace dae
{
	class Minigin
	{
	public:
		void Initialize();
		void LoadGame();
		void Cleanup();
		void Run();

		void enableAudioLogging();

	private:
		static const int MsPerFrame = 16; //16 for 60 fps, 33 for 30 fps
		SDL_Window*      m_Window{};

		std::shared_ptr<PeterPepperGameObject> peterPepper;
	};
}
