#pragma once
#include <iostream>

#include "Audio.h"

namespace dae
{
	class LoggedAudio : public Audio
	{
	public:
		explicit LoggedAudio(Audio& wrapped)
			: wrapped_(wrapped)
		{
		}

		void PlaySound(const int soundID) override
		{
			Log("Play sound. Sound ID: " + soundID);
			wrapped_.PlaySound(soundID);
		}

		void StopSound(const int soundID) override
		{
			Log("Stop sound. Sound ID: " + soundID);
			wrapped_.StopSound(soundID);
		}

		void StopAllSounds() override
		{
			Log("stop all sounds");
			wrapped_.StopAllSounds();
		}

		bool TogglePause() override
		{
			Log("pause all sounds");
			return wrapped_.TogglePause();
		}

	private:
		void Log(const char* message)
		{
			std::cout << "[Info] " << message << std::endl;
			// Code to log message...
		}

		Audio& wrapped_;
	};
}
