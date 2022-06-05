#pragma once
#include "Event.h"

namespace dae
{
	class EventHandler
	{
	public:
		virtual      ~EventHandler() = default;

		virtual void HandleEvent([[maybe_unused]] const Event* pEvent)
		{
		};
	};
}
