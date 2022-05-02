#pragma once
#include <SDL_mixer.h>

#include "Singleton.h"

namespace dae
{
	class Texture2D;
	class Font;

	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void                                     Init(const std::string& data);
		[[nodiscard]] std::shared_ptr<Texture2D> LoadTexture(const std::string& file) const;
		[[nodiscard]] std::shared_ptr<Font>      LoadFont(const std::string& file, unsigned int size) const;
		[[nodiscard]] Mix_Chunk*                 LoadAudio(const std::string& file) const;
	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;
		std::string m_DataPath;
	};
}
