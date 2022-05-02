#pragma once
#include <functional>

#include "Singleton.h"

#include <thread>

#include "ConcurrentQueue.h"

namespace dae
{
	enum Messages
	{
		PlaySound,
		StopSound,
		PauseSounds,
		StopSounds
	};

	struct MessageData
	{
		int soundID;
	};

	class Message
	{
	public:
		Message(const int id, const MessageData data) : m_Id(id), m_Data(std::move(data))
		{
		}

		int         GetID() { return m_Id; }
		MessageData GetData() { return m_Data; }
	private:
		int         m_Id;
		MessageData m_Data{};
	};


	class AudioEventQueue final : public Singleton<AudioEventQueue>
	{
	public:
		AudioEventQueue() : th([&] { ProcessQueue(); })
		{
		}

		void ProcessQueue()
		{
			Message* message;
			while (true)
			{
				queue.WaitAndPop(message);

				switch (message->GetID())
				{
				case PlaySound:
					Locator::GetAudio().PlaySound(message->GetData().soundID);
					break;
				case StopSound:
					Locator::GetAudio().StopSound(message->GetData().soundID);
					break;
				case PauseSounds:
					Locator::GetAudio().TogglePause();
				}
			}
		}

		void SendEvent(const int eventId, const MessageData data)
		{
			queue.Push(new Message(eventId, data));
		}

	private:
		friend class Singleton<AudioEventQueue>;

		std::jthread th;

		ConcurrentQueue<Message*> queue{};
	};
}
