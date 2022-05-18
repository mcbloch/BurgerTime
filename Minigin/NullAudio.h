#pragma once
#include "Audio.h"

namespace dae
{
	class NullAudio final : public Audio
	{
	public:
		void PlaySound(const int soundID) override
		{
			std::cout << "Null audio. No audio service specified." << std::endl;
			std::cout << "Trying to play sound: " << soundID << std::endl;
			/* Do nothing. */
		}

		void StopSound(const int soundID) override
		{
			std::cout << "Null audio. No audio service specified." << std::endl;
			std::cout << "Trying to stop sound: " << soundID << std::endl;
			/* Do nothing. */
		}

		void StopAllSounds() override
		{
			std::cout << "Null audio. No audio service specified." << std::endl;
			std::cout << "Trying to stop all sounds" << std::endl;
			/* Do nothing. */
		}

		bool TogglePause() override
		{
			std::cout << "Null audio. No audio service specified." << std::endl;
			std::cout << "Trying to pause all sounds" << std::endl;
			return false;
			/* Do nothing. */
		}
	};
}
