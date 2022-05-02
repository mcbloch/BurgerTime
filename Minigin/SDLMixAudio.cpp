#include "MiniginPCH.h"
#include "SDLMixAudio.h"

#include <SDL_mixer.h>

#include "ResourceManager.h"

class dae::SDLMixAudio::Impl
{
public:
	static void PrintSDLMixerVersion()
	{
		SDL_version        compileVersion;
		const SDL_version* linkVersion = Mix_Linked_Version();
		SDL_MIXER_VERSION(&compileVersion);
		printf("compiled with SDL_mixer version: %d.%d.%d\n",
		       compileVersion.major,
		       compileVersion.minor,
		       compileVersion.patch);
		printf("running with SDL_mixer version: %d.%d.%d\n",
		       linkVersion->major,
		       linkVersion->minor,
		       linkVersion->patch);
	}

	static void MixInit()
	{
		// Load support for other file formats if needed
		// constexpr int flags = MIX_INIT_FLAC | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MOD;
		constexpr int flags   = 0;
		const int     initted = Mix_Init(flags);
		if ((initted & flags) != flags)
		{
			printf("Mix_Init: Failed to init required ogg and mod support!\n");
			printf("Mix_Init: %s\n", Mix_GetError());
			// handle error
		}
	}

	static void OpenAudio()
	{
		// Initialize the Mixer Api
		constexpr int stereoAudio = 2; // mono = 1
		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, stereoAudio, 4096) == -1)
		{
			throw std::runtime_error(std::string(
				"SDL2_mixer could not be initialized!\n"
				"SDL_Error: ") + Mix_GetError());
		};
	}

	static void CloseAudio()
	{
		std::cout << "Closing audio" << std::endl;
		Mix_CloseAudio();

		// Cleanup for an unknown number of initializations.
		// Mix_Init can be called with different flags multiple times
		while (Mix_Init(0))
			Mix_Quit();
	}

	static void PlaySound(const int soundID)
	{
		const auto sample = ResourceManager::GetInstance().LoadAudio(soundFiles[soundID]);

		// play sample on first free unreserved channel
		// play it exactly once through. Passing -1 to 'loops' loops infinitely.
		// Mix_Chunk *sample; // previously loaded
		if (const int channel = Mix_PlayChannel(-1, sample, 0); channel == -1)
		{
			printf("Mix_PlayChannel: %s\n", Mix_GetError());
			Mix_FreeChunk(sample);
			// may be critical error, or maybe just no channels were free.
			// you could allocated another channel in that case...
		}
		else
		{
			// printf("Chunk info: \n");
			// printf("volume: %d\n", sample->volume);
			// printf("alen: %d\n", sample->alen);
			// printf("allocated: %d\n\n", sample->allocated);

			// printf("%d channels are playing\n", Mix_Playing(-1));
			// printf("Started channel: %d\n", Mix_Playing(channel));

			// get the last chunk used by channel
			printf("Mix_Chunk* last in use on channel %d was: %08p\n", channel, Mix_GetChunk(channel));

			// const int previous_volume = Mix_VolumeChunk(sample, MIX_MAX_VOLUME / 2);
			// printf("previous_volume: %d\n", previous_volume);
		}
	}

	static bool TogglePause()
	{
		// check the pause status on all channels
		const int pausedCount = Mix_Paused(-1);
		printf("%d channels are paused\n", pausedCount);

		if (pausedCount == 0)
		{
			// pause all sample playback
			Mix_Pause(-1);
			return true;
		}

		// resume playback on all previously active channels
		Mix_Resume(-1);
		return false;
	}

	static void StopSound(const int soundID)
	{
		Mix_Chunk* c = Mix_GetChunk(soundID);
		Mix_FreeChunk(c);
	}
};

dae::SDLMixAudio::SDLMixAudio() :
	pimpl{std::make_unique<Impl>()}
{
	pimpl->PrintSDLMixerVersion();
	pimpl->MixInit();
	pimpl->OpenAudio();
}

dae::SDLMixAudio::~SDLMixAudio()
{
	pimpl->CloseAudio();
}

void dae::SDLMixAudio::PlaySound(const int soundID)
{
	pimpl->PlaySound(soundID);
}

void dae::SDLMixAudio::StopSound(const int soundID)
{
	pimpl->StopSound(soundID);
	throw std::logic_error("Not implemented");
}

void dae::SDLMixAudio::StopAllSounds()
{
	throw std::logic_error("Not implemented");
}

/**
 * \brief 
 * \return True if the audio is now paused. False if resumed
 */
bool dae::SDLMixAudio::TogglePause()
{
	return pimpl->TogglePause();
}
