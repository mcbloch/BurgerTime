#pragma once

namespace dae
{
#define CANTINA_BAND 0
#define PINK_PANTHER 1

	class Audio
	{
	public:
		virtual ~Audio() = default;

		virtual void PlaySound(int soundID) = 0;
		virtual bool TogglePause() = 0;
		virtual void StopSound(int soundID) = 0;
		virtual void StopAllSounds() = 0;

	protected:
		inline static const std::vector<std::string> soundFiles{
			"CantinaBand60.wav",
			"PinkPanther60.wav"
		};
	};
}
