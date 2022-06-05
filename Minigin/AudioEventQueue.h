#pragma once
#include <functional>

#include "Singleton.h"

#include <thread>

#include "ConcurrentQueue.h"

namespace dae
{
	enum MessageType
	{
		PlaySound,
		StopSound,
		PauseSounds,
		StopSounds,
		StopAllSounds
	};

	struct MessageData
	{
		int soundID;
	};

	class Message
	{
	public:
		Message(const MessageType id, const MessageData data) : m_Id(id), m_Data(data)
		{
		}

		MessageType GetID() { return m_Id; }
		MessageData GetData() { return m_Data; }
	private:
		MessageType m_Id;
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
					break;
				case StopAllSounds:
					Locator::GetAudio().StopAllSounds();
					break;
				}
			}
		}

		void SendEvent(const MessageType eventId, const MessageData data)
		{
			queue.Push(new Message(eventId, data));
		}

	private:
		friend class Singleton<AudioEventQueue>;

		std::jthread th;

		ConcurrentQueue<Message*> queue{};
	};
}
