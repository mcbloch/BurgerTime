#pragma once

#include "Audio.h"

namespace dae
{
	class SDLMixAudio : public Audio
	{
	public:
		SDLMixAudio();
		~SDLMixAudio() override;

		void PlaySound(int soundID) override;
		void StopSound(int soundID) override;
		void StopAllSounds() override;
		bool TogglePause() override;

	private:
		class Impl;
		std::unique_ptr<Impl> pimpl;
	};
}
