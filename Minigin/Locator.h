#pragma once

#include "Audio.h"
#include "NullAudio.h"

namespace dae
{
	class Locator
	{
	public:
		inline static void Initialize() { service_ = &nullService_; }

		inline static Audio& GetAudio() { return *service_; }

		inline static void Provide(Audio* service)
		{
			if (service == nullptr)
			{
				// Revert to null service.
				service_ = &nullService_;
			}
			else
			{
				service_ = service;
			}
		}

	private:
		inline static Audio*    service_;
		inline static NullAudio nullService_;
	};
};
