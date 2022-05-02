#pragma once

namespace dae
{
#define CANTINA_BAND 0
#define PINK_PANTHER 1

#define BT_START 2
#define BT_MUSIC 3
#define BT_LEVEL_END 4
#define BT_LOSE_LIFE 5
#define BT_PAUSE_ITEM 6
#define BT_BONUS 7

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
			"PinkPanther60.wav",
			"01 Start.flac",
			"02 Game Music.flac",
			"03 Level End.flac",
			"04 Lose Life.flac",
			"05 Pause, Item Appears.flac",
			"06 Bonus.flac"
		};
	};
}
